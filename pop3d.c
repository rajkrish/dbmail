/* $Id$
 * (c) 2000-2001 IC&S, The Netherlands
 * 
 * pop3 daemon */

#include "pop3.h"
#include "dbmysql.h"

#define INCOMING_BUFFER_SIZE 512
#define IP_ADDR_MAXSIZE 16
#define APOP_STAMP_SIZE 255
/* default timeout for server daemon */
#define DEFAULT_SERVER_TIMEOUT 100

#ifndef SHUT_RDWR
#define SHUR_RDWR 3
#endif

/* syslog */
#define PNAME "dbmail/pop3d"

int state;
char *username=NULL, *password=NULL;
struct session curr_session;
char *myhostname;
char *apop_stamp;
char *timeout_setting;

char *buffer;

int resolve_client = 0;

int server_timeout;
int server_pid;

int error_count;

FILE *tx = NULL;	/* write socket */
FILE *rx = NULL;	/* read socket */

int *default_children;
int total_children = 0;

int done;

key_t	shmkey = 0;
int shmid;

#define SHM_ALLOC_SIZE (sizeof(int))

/* signal handler */
static void signal_handler (int signo)
{
  pid_t PID;
  int status;

  if ((signo == SIGALRM) && (tx!=NULL))
  {
		done=-1;
		trace (TRACE_DEBUG,"signal_handler(): received ALRM signal. Timeout");
		fprintf (tx,"-ERR i cannot wait forever\r\n");
		fflush (tx);
		shutdown(fileno(tx),SHUT_RDWR);
		shutdown(fileno(rx),SHUT_RDWR);
		return;
  }
  else
	 if (signo == SIGCHLD)
	  {
		  trace (TRACE_DEBUG,"signal_handler(): sigCHLD, cleaning up zombies");
		  do {
			  PID = waitpid (-1,&status,WNOHANG);
		  } while ( PID != -1);

		  trace (TRACE_DEBUG,"signal_handler(): sigCHLD, cleaned");
		  signal (SIGCHLD, signal_handler);
	    return;
	  }
	  else
		  trace (TRACE_STOP,"signal_handler(): received fatal signal [%d]",signo);
}

int handle_client(char *myhostname, int c, struct sockaddr_in adr_clnt)
{
	 /* returns 0 when a connection was successfull
	 * returns -1 when a connection was unsuccessfull (continue in loop)
	 */

  char *theiraddress;
  char *buffer;
  
  struct hostent *clientinfo;
  
  time_t timestamp;
  time_t timeout;

  /* reset */
  done = 1;
  if (username != NULL)
  {
	  free(username);
	  username = NULL;
  }
  if (password != NULL)
  {
	  free(password);
	  password = NULL;
  }

  curr_session.validated=0;
  /* done resetting */


  
  theiraddress=inet_ntoa(adr_clnt.sin_addr);

  memtst((clientinfo=(struct hostent *)malloc(1))==NULL);

  if (resolve_client==1)
	{
		clientinfo=gethostbyaddr((char *)&adr_clnt.sin_addr, 
		sizeof(adr_clnt.sin_addr),
			adr_clnt.sin_family);
		if (theiraddress != NULL)
			trace (TRACE_MESSAGE,"handle_client(): incoming connection from [%s (%s)]",
						theiraddress,clientinfo->h_name);
		else
		{
			trace (TRACE_ERROR,"handle_client(): error: could not get address of client"); 
			return -1;
		}
	}
	else
	{
		if (theiraddress != NULL)
			trace (TRACE_MESSAGE,"handle_client(): incoming connection from [%s]",
				theiraddress);
		else
			trace (TRACE_ERROR,"handle_client(): error: could not get address of client");
	}

	/* duplicate descriptor and open it */
	rx = fdopen (dup (c), "r"); 
	if (!rx)
	{
		/* opening of descriptor failed */
		close(c);
		return -1;
	}
	
	tx = fdopen (dup (c), "w"); 
	if (!tx)
	{
		/* opening of descriptor failed */
		close (c);
		return -1;
	}
		
	/* set stream to line buffered mode 
	* this way when we send a newline the buffer is flushed */
/*	setlinebuf(tx); */
	setlinebuf(rx);

	/* connect to the database */
	if (db_connect()< 0)
	{	
		trace(TRACE_ERROR,"handle_client(): could not connect to database");
		return -1;
	}
			
	/* first initiate AUTHORIZATION state */
	state = AUTHORIZATION;
		
	memtst((buffer=(char *)malloc(INCOMING_BUFFER_SIZE))==NULL);

	/* create an unique timestamp + processid for APOP authentication */
	memtst((apop_stamp=(char *)malloc(APOP_STAMP_SIZE))==NULL);
				
	timestamp=time(NULL);
				
	sprintf (apop_stamp,"<%d.%u@%s>",getpid(),timestamp,myhostname);

	/* sending greeting */
	fprintf (tx,"+OK DBMAIL server ready %s\r\n",apop_stamp);
	fflush (tx);
			
	/* no errors yet */
	error_count = 0;
			
	trace (TRACE_DEBUG,"handle_client(): setting timeout timer at %d seconds",server_timeout);	
	/* setting time for timeout counter */
	alarm (server_timeout); 

	/* scanning for commands */
	while ((done>0) && (buffer=fgets(buffer,INCOMING_BUFFER_SIZE,rx)))
	{
		if (feof(rx)) 
			done = -1;  /* check of client eof  */
		else 
		{
			alarm (server_timeout);  
			done = pop3(tx,buffer); 
			alarm (server_timeout); 
		}
	fflush (tx);
	}
					
	/* we've reached the update state */
	state = UPDATE;

	/* memory cleanup */
	free(buffer);

	if (done < 0)
	{
		trace (TRACE_ERROR,"handle_client(): timeout, connection terminated");
		fclose(tx);
		shutdown (fileno(rx), SHUT_RDWR);
		fclose(rx);
	}
	else
	{
		if (curr_session.validated == 0)
		{
			trace (TRACE_MESSAGE,"handle_client(): !alert somebody from [%s] tried to login as [%s]!",
			theiraddress, username);
		}
		else
			{		
			trace(TRACE_MESSAGE,"handle_client(): user %s logging out [message=%lu, octets=%lu]",
				username, curr_session.virtual_totalmessages,
				curr_session.virtual_totalsize);

			/* if everything went well, write down everything and do a cleanup */
			db_update_pop(&curr_session);
			}

		db_disconnect(); 
	
		fclose(tx);
		shutdown (fileno(rx), SHUT_RDWR);
		fclose(rx);
	}

	/* reset timers */
	alarm (0);
	
	return 0;
}

int main (int argc, char *argv[])
{
  struct sockaddr_in adr_srvr;
  struct sockaddr_in adr_clnt;
  char *myhostname;

  char *newuser, *newgroup;
  
  char *ipaddr, *port;

  char *trace_level=NULL,*trace_syslog=NULL,*trace_verbose=NULL;
  int new_level = 2, new_trace_syslog = 1, new_trace_verbose = 0;
  char *resolve_setting=NULL;
  
  pid_t processid;
	  
  int len_inet;
  int reuseaddress;
  int s = -1;
  int c = -1;
  int z, i; /* counters */
  int defchld,maxchld; /* default children and maxchildren */

  /* open logs */
  openlog(PNAME, LOG_PID, LOG_MAIL);
  
  /* connect to the database */
  if (db_connect()< 0) 
	trace(TRACE_FATAL,"main(): could not connect to database"); 
	
  /* debug settings */
  trace_level = db_get_config_item("TRACE_LEVEL", CONFIG_EMPTY);
  trace_syslog = db_get_config_item("TRACE_TO_SYSLOG", CONFIG_EMPTY);
  trace_verbose = db_get_config_item("TRACE_VERBOSE", CONFIG_EMPTY);
  timeout_setting = db_get_config_item("POP3D_CHILD_TIMEOUT", CONFIG_EMPTY);
  resolve_setting = db_get_config_item("POP3D_IP_RESOLVE",CONFIG_EMPTY);

  if (resolve_setting)
  {
		if (strcasecmp(resolve_setting,"yes"))
				resolve_client = 1;
		else
				resolve_client = 0;
		free(resolve_setting);
  }	
  
  if (timeout_setting) 
  {
	  server_timeout = atoi(timeout_setting);
	  free (timeout_setting);
	  if (server_timeout<10)
		  trace (TRACE_STOP,"main(): POP3D_CHILD_TIMEOUT setting is insane [%d]",
				  server_timeout);
  }
  else
	  server_timeout = DEFAULT_SERVER_TIMEOUT;
  
  if (trace_level)
    {
      new_level = atoi(trace_level);
      free(trace_level);
      trace_level = NULL;
    }

  if (trace_syslog)
    {
      new_trace_syslog = atoi(trace_syslog);
      free(trace_syslog);
      trace_syslog = NULL;
    }

  if (trace_verbose)
    {
      new_trace_verbose = atoi(trace_verbose);
      free(trace_verbose);
      trace_verbose = NULL;
    }

  configure_debug(new_level, new_trace_syslog, new_trace_verbose);


  /* daemonize */
  if (fork ())
    exit (0);
		
  close (0);
  close (1);
  close (2); 
  close (3);
  
  /* reserve memory for hostname */
  memtst((myhostname=(char *)malloc(64))==NULL);
	
  /* getting hostname */
  gethostname (myhostname,64);
	
  /* set signal handler for SIGCHLD */
  signal (SIGCHLD, signal_handler);
  signal (SIGINT, signal_handler);
  signal (SIGQUIT, signal_handler);
  signal (SIGILL, signal_handler);
  signal (SIGKILL, signal_handler);
  signal (SIGBUS, signal_handler);
  signal (SIGFPE, signal_handler);
  signal (SIGSEGV, signal_handler);
  signal (SIGTERM, signal_handler);
  signal (SIGSTOP, signal_handler);
  signal (SIGALRM, signal_handler);

  adr_srvr.sin_family = AF_INET; 
  
  s = socket (PF_INET, SOCK_STREAM, 0); /* creating the socket */
  if (s == -1 ) 
    trace (TRACE_FATAL,"main(): call socket(2) failed");

  ipaddr = db_get_config_item("POP3D_BIND_IP",CONFIG_MANDATORY);
  port = db_get_config_item("POP3D_BIND_PORT",CONFIG_MANDATORY);

  if (ipaddr != NULL)
  {
	  if (ipaddr[0] == '*') /* bind to all interfaces */
			adr_srvr.sin_addr.s_addr = htonl (INADDR_ANY); 
	  else 
		{
		   if (!inet_aton(ipaddr, &adr_srvr.sin_addr))
				trace (TRACE_FATAL, "main(): %s is not a valid ipaddress",ipaddr);
		}
  }
  else
	  trace (TRACE_FATAL,"main(): could not read bind ip from config");

  if (port != NULL)
	  adr_srvr.sin_port = htons (atoi(port));
	else
		trace (TRACE_FATAL,"main(): could not read port from config");
			
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuseaddress, sizeof(reuseaddress));
	
  len_inet = sizeof (adr_srvr);

  z = bind (s, (struct sockaddr *)&adr_srvr, len_inet); /* bind to socket */
  if (z == -1 )
    trace (TRACE_FATAL,"main(): call bind(2) failed (could not bind to %s:%s)",ipaddr,port);

  z = listen (s, BACKLOG); /* make the socket listen */
  if (z == -1 )
    trace (TRACE_FATAL,"main(): call listen(2) failed");

  /* drop priviledges */
	trace (TRACE_MESSAGE,"main(): Dropping priviledges");		

	
  newuser = db_get_config_item("POP3D_EFFECTIVE_USER",CONFIG_MANDATORY);
  newgroup = db_get_config_item("POP3D_EFFECTIVE_GROUP",CONFIG_MANDATORY);
	
  if ((newuser!=NULL) && (newgroup!=NULL))
  {
    if (drop_priviledges (newuser, newgroup) != 0)
      trace (TRACE_FATAL,"main(): could not set uid %s, gid %s",newuser,newgroup);
    
    free(newuser);
    free(newgroup);
    newuser = NULL;
    newgroup = NULL;
  }
  else
    trace(TRACE_FATAL,"main(): newuser and newgroup should not be NULL");

  /* getting shared memory children counter */
  shmkey = time (NULL); /* get an unique key */
  shmid = shmget (shmkey, sizeof (int), 0666 | IPC_CREAT);

  if (shmid == -1)
	  trace (TRACE_FATAL,"main(): could not allocate shared memory");


  /* server loop */
  trace (TRACE_MESSAGE,"main(): DBmail pop3 server ready (bound to [%s:%s])",ipaddr,port);
	
	free (ipaddr);
	free (port);
  
 
 /* get child config */

  defchld = atoi(db_get_config_item("POP3D_DEFAULT_CHILD",CONFIG_MANDATORY));
  maxchld = atoi(db_get_config_item("POP3D_MAX_CHILD",CONFIG_MANDATORY));


  /* remember this pid, we're the father process */
  server_pid = getpid();
  
  default_children = (int *)shmat(shmid, 0, 0);
  if (default_children == (int *)-1)
	  trace (TRACE_FATAL,"main(): could not attach to shared memory block");

	/* we don't have any children yet */
	*default_children = 0;
	
	/* spawn the default children */
	for (i=0; i<defchld; i++)
	{
		if (!fork())
			break;
		else
			total_children++;
	}

	/* split up in the 'server' part and the client part */

	/* 
	 * Client loop 
	 */
	if (getpid() != server_pid)
	{
		for (;;)
		{
			/* wait for a connection */
			len_inet = sizeof (adr_clnt);
			c = accept (s, (struct sockaddr *)&adr_clnt,
			  &len_inet); /* incoming connection */
	
			/* failure won't cause a quit forking is too expensive */	
			if (c == -1)
			{
				trace (TRACE_FATAL,"main(): call accept(2) failed");
			}
		
		(*default_children)++;		
			
		handle_client(myhostname, c, adr_clnt);
		
		(*default_children)--;
		}
	}
	else
	for (;;)
	{
		if (*default_children < defchld)
		  {
			sleep (1); /* don't hog cpu */
			continue;
		  }

		while (total_children >= maxchld)
		{
			sleep (1); /* don't hog cpu */
			wait (NULL); /* wait for children to finish */
			total_children--;
		}

		/* wait for a connection */
		len_inet = sizeof (adr_clnt);
		c = accept (s, (struct sockaddr *)&adr_clnt,
			  &len_inet); /* incoming connection */
	
		/* failure won't cause a quit forking is too expensive */	
		if (c == -1)
		{
			trace (TRACE_FATAL,"main(): call accept(2) failed");
		}
		
		if (fork())
		{
			total_children++;
			continue;
		}
		else
		{
		/* handle client connection */
		handle_client(myhostname, c, adr_clnt);
		exit(0);
		}
	}
	/* nothing will ever get here */
	return 0;
}		
