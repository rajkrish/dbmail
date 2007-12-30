/*
 *  Copyright (c) 2005-2006 NFG Net Facilities Group BV support@nfg.nl
 *
 *  licence: GPLv2
 *  see COPYING for details.
 *  
 */

#include "dbmail.h"
#define THIS_MODULE "check"

char *configFile = "/etc/dbmail/dbmail-test.conf";


/* simple testmessages. */
char *simple = "From nobody Wed Sep 14 16:47:48 2005\n"
	"X-Foo: bar\n"
	"Subject: dbmail test message\n"
	"\n"
	"\n"
	"    this is a test message\n"
	"\n";

char *simple_with_from = "From nobody@pacific.net.sg Tue Dec 04 19:52:17 2007\n"
	"X-Envelope-From: <nobody@pacific.net.sg>\n"
	"Received: from [127.0.0.1] (port=49353 helo=test11)\n"
	"        by centos.nowhere.com with smtp (Exim 4.63)\n"
	"        (envelope-from <nobody@pacific.net.sg>)\n"
	"        id 1IzWJv-0000Ep-5f\n"
	"        for wallace@nowhere.com; Tue, 04 Dec 2007 19:52:17 +0800\n"
	"From: \"Wallace\" <nobody@pacific.net.sg>\n"
	"To: wallace <wallace@nowhere.com>\n"
	"Subject: Test 11\n"
	"Message-Id: <E1IzWJv-0000Ep-5f@centos.nowhere.com>\n"
	"Date: Tue, 04 Dec 2007 19:52:16 +0800\n"
	"\n"
	"\n"
	"This line works, however,\n"
	"From what I know, this line gets truncated\n"
	"This line gets truncated\n"
	"This other line get truncated too\n";

char *rfc822 = "From nobody Wed Sep 14 16:47:48 2005\n" 
	"Content-Type: text/plain; charset=\"us-ascii\"\n"
	"MIME-Version: 1.0\n"
	"Content-Transfer-Encoding: 7bit\n"
	"To: testuser@foo.org\n"
	"From: somewher@foo.org\n"
	"Subject: dbmail test message\n"
	"\n"
	"\n"
	"    this is a test message\n"
	"\n";
    
char * utf7_header = "From: \"Some Body\" <s.body@freeler.nl>\n"
	"Subject: simple subject\n"
	"To: =?iso-8859-1?Q?::_=5B_Arrty_=5D_::_=5B_Roy_=28L=29_St=E8phanie_=5D?=  <over.there@hotmail.com>\n"
	"Date: Thu, 11 Oct 2007 19:00:37 +0200\n"
	"MIME-Version: 1.0\n"
	"\n"
	" test message\n"
	"\n";

char *multipart_message = "From: \"Brother from another planet\" <vol@inter7.com>\n"
	"To: \"Brother from another planet\" <vol@inter7.com>\n"
	"Reply-to: \"Brother from another planet\" <vol@inter7.com>\n"
	"Cc: \"Brother from another planet\" <vol@inter7.com>,\n"
	" \"SpongeBob O'Brien\" <nobody@test123.com>\n"
	"Subject: multipart/mixed\n"
	"References: <5.1.0.14.0.20020926105718.01c0a568@mail>\n"
	"               <5.1.0.14.0.20020926105718.01c0a568@mail>\n"
	"Date: Wed, 11 May 2005 13:20:08 -0700\n"
	"Received: at mx.inter7.com from localhost\n"
	"Received: at localhost from localhost\n"
	"Received: at localhost from localhost\n"
	"MIME-Version: 1.0\n"
	"Content-type: multipart/mixed; boundary=boundary\n"
	"X-Dbmail-ID: 12345\n"
	"\n"
	"MIME multipart messages specify that there are multiple\n"
	"messages of possibly different types included in the\n"
	"message.  All peices will be availble by the user-agent\n"
	"if possible.\n"
	"\n"
	"The header 'Content-disposition: inline' states that\n"
	"if possible, the user-agent should display the contents\n"
	"of the attachment as part of the email, rather than as\n"
	"a file, or message attachment.\n"
	"\n"
	"(This message will not be seen by the user)\n"
	"\n"
	"--boundary\n"
	"Content-type: text/html\n"
	"Content-disposition: inline\n"
	"\n"
	"Test message one\n"
	"--boundary\n"
	"Content-type: text/plain; charset=us-ascii; name=testfile\n"
	"Content-transfer-encoding: base64\n"
	"\n"
	"IyEvYmluL2Jhc2gNCg0KY2xlYXINCmVjaG8gIi4tLS0tLS0tLS0tLS0tLS0t\n"
	"LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS4i\n"
	"DQplY2hvICJ8IE1hcmNoZXcuSHlwZXJyZWFsIHByZXNlbnRzOiB2aXhpZSBj\n"
	"cm9udGFiIGV4cGxvaXQgIzcyODM3MSB8Ig0KZWNobyAifD09PT09PT09PT09\n"
	"PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09\n"
	"PT09fCINCmVjaG8gInwgU2ViYXN0aWFuIEtyYWhtZXIgPGtyYWhtZXJAc2Vj\n"
	"dXJpdHkuaXM+ICAgICAgICAgICAgICAgICAgIHwiDQplY2hvICJ8IE1pY2hh\n"
	"--boundary--\n";

//
char *multipart_alternative = "From paul@nfg.nl Tue Oct 11 13:06:24 2005\n"
	"Message-ID: <43E5FE98.4030609@nfg.nl>\n"
	"Date: Sun, 05 Feb 2006 14:33:12 +0100\n"
	"From: Paul J Stevens <paul@nfg.nl>\n"
	"Organization: NFG Net Facilities Group BV\n"
	"User-Agent: Debian Thunderbird 1.0.2 (X11/20051002)\n"
	"X-Accept-Language: en-us, en\n"
	"To: paul@nfg.nl\n"
	"Subject: test\n"
	"X-Enigmail-Version: 0.91.0.0\n"
	"X-DBMail-PhysMessage-ID: 841010\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed; boundary=------------050000030206040804030909\n"
	"\n"
	"This is a multi-part message in MIME format.\n"
	"--------------050000030206040804030909\n"
	"Content-Type: multipart/alternative; boundary=\"------------040302030903000400040101\"\n"
	"\n"
	"\n"
	"--------------040302030903000400040101\n"
	"Content-Type: text/plain; charset=ISO-8859-1\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"\n"
  	"test\n"
	"\n"
	"-- \n"
  	"________________________________________________________________\n"
  	"Paul Stevens                                  mailto:paul@nfg.nl\n"
  	"NET FACILITIES GROUP                     PGP: finger paul@nfg.nl\n"
  	"The Netherlands________________________________http://www.nfg.nl\n"
	"\n"
	"\n"
	"--------------040302030903000400040101\n"
	"Content-Type: text/html; charset=ISO-8859-1\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
	"<html>\n"
	"<head>\n"
  	"<meta content=\"text/html;charset=ISO-8859-1\" http-equiv=\"Content-Type\">\n"
  	"<title></title>\n"
	"</head>\n"
	"<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
	"<h1>test</h1>\n"
	"<pre class=\"moz-signature\" cols=\"72\">-- \n"
  	"________________________________________________________________\n"
  	"Paul Stevens                                  <a class=\"moz-txt-link-freetext\" href=\"mailto:paul@nfg.nl\">mailto:paul@nfg.nl</a>\n"
  	"NET FACILITIES GROUP                     PGP: finger <a class=\"moz-txt-link-abbreviated\" href=\"mailto:paul@nfg.nl\">paul@nfg.nl</a>\n"
  	"The Netherlands________________________________<a class=\"moz-txt-link-freetext\" href=\"http://www.nfg.nl\">http://www.nfg.nl</a>\n"
	"</pre>\n"
	"</body>\n"
	"</html>\n"
	"\n"
	"--------------040302030903000400040101--\n"
	"\n"
	"--------------050000030206040804030909\n"
	"Content-Type: image/jpeg; name=\"jesse_2.jpg\"\n"
	"Content-Transfer-Encoding: base64\n"
	"Content-Disposition: inline; filename=\"jesse_2.jpg\"\n"
	"\n"
	"/9j/4AAQSkZJRgABAQEASABIAAD/4RirRXhpZgAASUkqAAgAAAAJAA8BAgAGAAAAegAAABAB\n"
	"AgAWAAAAgAAAABIBAwABAAAABgAAABoBBQABAAAAlgAAABsBBQABAAAAngAAACgBAwABAAAA\n"
	"4SSSAjbtIaCVfBmeySSaBgF/QFxTe0wJwuA1XS6lm9zCPS6TTckkozVoEzHvqhq6WySQ6mYI\n"
	"7LF3kd0klwPUfuX7HS032seT7fokkkufZpP/2Q==\n"
	"--------------050000030206040804030909--\n";

//
char *outlook_multipart = "From aprilbabies-bounces@lists.nfg.nl  Fri Nov 25 22: 34:35 2005\n"
	"From: \"Foo Bar\" <foobar@foo.bar>\n"
	"To: \"My List...\" <mylist@foo.bar>\n"
	"Subject: RE: [MyList] blah\n"
	"Date: Fri, 25 Nov 2005 22:34:16 +0100\n"
	"Message-ID: <IFEIKJKAMPLPGJIEIMEECEIFEDAA.ka@sus.se>\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed;\n"
	"	boundary=\"===============0825346837==\"\n"
	"\n"
	"--===============0825346837==\n"
	"Content-Type: text/plain;\n"
	"	charset=\"Windows-1252\"\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"test\n"
	"\n"
	"\n"
	"--===============0825346837==\n"
	"Content-Type: text/plain; charset=\"iso-8859-1\"\n"
	"MIME-Version: 1.0\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"Content-Disposition: inline\n"
	"\n"
	"my sig.\n"
	"--===============0825346837==--\n"
	"\n";

/* raw_lmtp_data is equal to multipart_message, except for the line-endings
 * and the termination dot.
 */
char *raw_lmtp_data = "From: \"Brother from another planet\" <vol@inter7.com>\r\n"
	"To: \"Brother from another planet\" <vol@inter7.com>\r\n"
	"Subject: multipart/mixed\r\n"
	"Reply-to: \"Brother from another planet\" <vol@inter7.com>\r\n"
	"Cc: \"Brother from another planet\" <vol@inter7.com>,\r\n"
	" \"SpongeBob O'Brien\" <nobody@test123.com>\r\n"
	"Date: Wed, 11 May 2005 13:20:08 -0700\n"
	"Received: at mx.inter7.com from localhost\r\n"
	"Received: at localhost from localhost\r\n"
	"MIME-Version: 1.0\r\n"
	"Content-type: multipart/mixed; boundary=\"boundary\"\r\n"
	"X-Dbmail-ID: 12345\r\n"
	"\r\n"
	"MIME multipart messages specify that there are multiple\r\n"
	"messages of possibly different types included in the\r\n"
	"message.  All peices will be availble by the user-agent\r\n"
	"if possible.\r\n"
	"\r\n"
	"The header 'Content-disposition: inline' states that\r\n"
	"if possible, the user-agent should display the contents\r\n"
	"of the attachment as part of the email, rather than as\r\n"
	"a file, or message attachment.\r\n"
	"\r\n"
	"(This message will not be seen by the user)\r\n"
	"\r\n"
	"--boundary\r\n"
	"Content-type: text/html\r\n"
	"Content-disposition: inline\r\n"
	"\r\n"
	"Test message one\r\n"
	"--boundary\r\n"
	"Content-type: text/plain; charset=us-ascii; name=\"testfile\"\r\n"
	"Content-transfer-encoding: base64\r\n"
	"\r\n"
	"IyEvYmluL2Jhc2gNCg0KY2xlYXINCmVjaG8gIi4tLS0tLS0tLS0tLS0tLS0t\r\n"
	"LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS4i\r\n"
	"DQplY2hvICJ8IE1hcmNoZXcuSHlwZXJyZWFsIHByZXNlbnRzOiB2aXhpZSBj\r\n"
	"cm9udGFiIGV4cGxvaXQgIzcyODM3MSB8Ig0KZWNobyAifD09PT09PT09PT09\r\n"
	"PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09\r\n"
	"PT09fCINCmVjaG8gInwgU2ViYXN0aWFuIEtyYWhtZXIgPGtyYWhtZXJAc2Vj\r\n"
	"dXJpdHkuaXM+ICAgICAgICAgICAgICAgICAgIHwiDQplY2hvICJ8IE1pY2hh\r\n"
	"--boundary--\r\n"
	".\r\n";

char *simple_message_part = "Content-Type: text/plain; charset=\"iso-8859-1\"\n"
	"MIME-Version: 1.0\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"Content-Disposition: inline\n"
	"\n"
	"my sig.\n";

char *multipart_message_part = "Content-Type: text/plain;\n"
	" name=\"mime_alternative\"\n"
	"Content-Transfer-Encoding: 7bit\n"
	"Content-Disposition: inline;\n"
	" filename=\"mime_alternative\"\n"
	"\n"
	"From: <vol@inter7.com>\n"
	"To: <vol@inter7.com>\n"
	"Subject: multipart/alternative\n"
	"MIME-Version: 1.0\n"
	"Content-type: multipart/alternative; boundary=\"boundary\"\n"
	"\n"
	"MIME alternative sample body\n"
	"(user never sees this portion of the message)\n"
	"\n"
	"These messages are used to send multiple versions of the same\n"
	"message in different formats.  User-agent will decide which\n"
	"to display.\n"
	"\n"
	"--boundary\n"
	"Content-type: text/html\n"
	"\n"
	"<HTML><HEAD><TITLE>HTML version</TITLE></HEAD><BODY>\n"
	"<CENTER>HTML version</CENTER>\n"
	"</BODY></HTML>\n"
	"--test\n"
	"Content-type: text/plain\n"
	"\n"
	"Text version\n"
	"--boundary--\n"
	"\n";

char *encoded_message_koi = "From: =?koi8-r?Q?=E1=CE=D4=CF=CE=20=EE=C5=C8=CF=D2=CF=DB=C9=C8=20?=<bad@foo.ru>\n"
	"To: nobody@foo.ru\n"
	"Subject: test\n"
	"MIME-Version: 1.0\n"
	"Content-Type: text/plain\n"
	"\n"
	"test mail\n\n";

char *raw_message_koi = "From: test@test\n"
	"To: test@foo.com\n"
	"Subject: �������� koi8-r ������\n"
	"\n"
	"������ ����\n"
	"\n";

char *encoded_message_latin_1 = "From: =?iso-8859-1?Q?B=BA_V._F._Z=EAzere?= <nobody@nowhere.org>\n"
	"To: nobody@foo.org\n"
	"Subject: =?iso-8859-1?Q?Re:_M=F3dulo_Extintores?=\n"
	"MIME-Version: 1.0\n"
	"Content-Type: text/plain\n"
	"\n"
	"test\n\n";

char *encoded_message_latin_2 = "From: =?ISO-8859-2?Q?=22Miroslav_=A9ulc_=28fordfrog=29=22?=\n"
	"	<fordfrog@gentoo.org>\n"
	"To: gentoo-dev@lists.gentoo.org\n"
	"Subject: =?ISO-8859-2?Q?Re=3A_=5Bgentoo-dev=5D_New_developer=3A__?=\n"
	" =?ISO-8859-2?Q?Miroslav_=A9ulc_=28fordfrog=29?=\n"
	"MIME-Version: 1.0\n"
	"Content-Type: text/plain;\n"
	"  charset=ISO-8859-2;\n"
	"  format=flowed\n"
	"\n"
	"test\n";

char *encoded_message_utf8 = "From: nobody@nowhere.org\n"
	"To: nobody@foo.org\n"
	"Subject: =?utf-8?b?w6nDqcOp?=\n"
	"MIME-Version: 1.0\n"
	"Content-Type: text/plain\n"
	"\n"
	"test\n\n";


char *multipart_mixed = "Received: from RAIVO (raivo.kisise [192.168.111.49])\n"
	"	by test.kisise (Postfix) with ESMTP id 5C4981214C\n"
	"	for <test@test.kisise>; Thu,  5 Oct 2006 04:18:26 +0300 (EEST)\n"
	"From: \"try\" <try@test.kisise>\n"
	"To: <test@test.kisise>\n"
	"Subject: 2 docs\n"
	"Date: Wed, 4 Oct 2006 18:52:09 +0300\n"
	"Message-ID: <005801c6e7cd$0d2888b0$316fa8c0@RAIVO>\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed;\n"
	"	boundary=\"----=_NextPart_A\"\n"
	"X-Priority: 3 (Normal)\n"
	"X-MSMail-Priority: Normal\n"
	"X-Mailer: Microsoft Outlook, Build 10.0.6626\n"
	"Importance: Normal\n"
	"X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.2962\n"
	"\n"
	"This is a multi-part message in MIME format.\n"
	"\n"
	"------=_NextPart_A\n"
	"Content-Type: multipart/alternative;\n"
	"	boundary=\"----=_NextPart_B\"\n"
	"\n"
	"\n"
	"------=_NextPart_B\n"
	"Content-Type: text/plain;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"\n"
	"This message contains 2 other messages with MS Word .doc attachments. =\n"
	"These\n"
	"two messages are forwarded with MS Outlook 2000, using HTML and MS Word =\n"
	"as\n"
	"the editor (default MS Outlook settings). This is the body of the new =\n"
	"third\n"
	"message. The first two messages were created in the same way with MS =\n"
	"Outlook\n"
	"in HTML in a MS Word window.\n"
	"\n"
	"\n"
	"------=_NextPart_B\n"
	"Content-Type: text/html;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"\n"
	"<html>\n"
	"\n"
	"<head>\n"
	"<META HTTP-EQUIV=3D\"Content-Type\" CONTENT=3D\"text/html; =\n"
	"charset=3Dus-ascii\">\n"
	"\n"
	"\n"
	"<meta name=3DGenerator content=3D\"Microsoft Word 10 (filtered)\">\n"
	"\n"
	"<style>\n"
	"<!--\n"
	" /* Style Definitions */\n"
	" p.MsoNormal, li.MsoNormal, div.MsoNormal\n"
	"	{margin:0cm;\n"
	"	margin-bottom:.0001pt;\n"
	"	font-size:12.0pt;\n"
	"	font-family:\"Times New Roman\";}\n"
	"a:link, span.MsoHyperlink\n"
	"	{color:blue;\n"
	"	text-decoration:underline;}\n"
	"a:visited, span.MsoHyperlinkFollowed\n"
	"	{color:purple;\n"
	"	text-decoration:underline;}\n"
	"span.Meililaad17\n"
	"	{font-family:Arial;\n"
	"	color:navy;}\n"
	"@page Section1\n"
	"	{size:595.3pt 841.9pt;\n"
	"	margin:70.85pt 70.85pt 70.85pt 70.85pt;}\n"
	"div.Section1\n"
	"	{page:Section1;}\n"
	"-->\n"
	"</style>\n"
	"\n"
	"</head>\n"
	"\n"
	"<body lang=3DET link=3Dblue vlink=3Dpurple>\n"
	"\n"
	"<div class=3DSection1>\n"
	"\n"
	"<p class=3DMsoNormal><font size=3D2 color=3Dnavy face=3DArial><span =\n"
	"style=3D'font-size:\n"
	"10.0pt;font-family:Arial;color:navy'>This message contains 2 other =\n"
	"messages with\n"
	"MS Word .doc attachments. These two messages are forwarded with MS =\n"
	"Outlook\n"
	"2000, using HTML and MS Word as the editor (default MS Outlook =\n"
	"settings). This is\n"
	"the body of the new third message. The first two messages were created =\n"
	"in the same\n"
	"way with MS Outlook in HTML in a MS Word window.</span></font></p>\n"
	"\n"
	"</div>\n"
	"\n"
	"</body>\n"
	"\n"
	"</html>\n"
	"\n"
	"------=_NextPart_B--\n"
	"\n"
	"------=_NextPart_A\n"
	"Content-Type: message/rfc822\n"
	"Content-Transfer-Encoding: 7bit\n"
	"Content-Disposition: attachment\n"
	"\n"
	"From: \"try\" <try@test.kisise>\n"
	"To: <try@test.kisise>\n"
	"Subject: doc2\n"
	"Date: Wed, 4 Oct 2006 17:56:28 +0300\n"
	"Message-ID: <000601c6e7c5$45735e00$316fa8c0@RAIVO>\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed;\n"
	"	boundary=\"----=_NextPart_C\"\n"
	"X-Priority: 3 (Normal)\n"
	"X-MSMail-Priority: Normal\n"
	"X-Mailer: Microsoft Outlook, Build 10.0.6626\n"
	"X-DSPAM-Result: Innocent\n"
	"X-DSPAM-Processed: Thu Oct  5 03:19:33 2006\n"
	"X-DSPAM-Confidence: 0.9899\n"
	"X-DSPAM-Improbability: 1 in 9809 chance of being spam\n"
	"X-DSPAM-Probability: 0.0000\n"
	"X-DSPAM-Signature: 28,45244f95213033639915435\n"
	"X-DSPAM-Factors: 27,size+10, 0.01000,Content-Type*charset=\"us, 0.01000,Content-Type*charset=\"us, 0.01000,X-Mailer*Outlook+Build, 0.01000,Received*<try+test.kisise>, 0.01000,10, 0.01000,10, 0.01000,To*test.kisise>, 0.01000,of, 0.01000,of, 0.01000,<font, 0.01000,Received*ESMTP, 0.01000,X-MimeOLE*By, 0.01000,message, 0.01000,X-Priority*3+(Normal), 0.01000,X-MimeOLE*Produced, 0.01000,(filtered)\">+<font, 0.01000,X-Mailer*Build+10.0.6626, 0.01000,multi, 0.01000,Received*Oct, 0.01000,Content-Transfer-Encoding*quoted+printable, 0.01000,content=\"Microsoft, 0.01000,Received*by+test.kisise, 0.01000,<meta, 0.01000,10+(filtered)\">, 0.01000,charset=us+ascii\">, 0.01000,X-MimeOLE*Microsoft+MimeOLE, 0.01000\n"
	"X-DBMail-PhysMessage-ID: 104\n"
	"Importance: Normal\n"
	"X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.2962\n"
	"\n"
	"This is a multi-part message in MIME format.\n"
	"\n"
	"------=_NextPart_C\n"
	"Content-Type: multipart/alternative;\n"
	"	boundary=\"----=_NextPart_D\"\n"
	"\n"
	"\n"
	"------=_NextPart_D\n"
	"Content-Type: text/plain;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"Body of doc2\n"
	"\n"
	"\n"
	"------=_NextPart_D\n"
	"Content-Type: text/html;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"\n"
	"<html>\n"
	"\n"
	"<head>\n"
	"<META HTTP-EQUIV=3D\"Content-Type\" CONTENT=3D\"text/html; =\n"
	"charset=3Dus-ascii\">\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"<meta name=3DGenerator content=3D\"Microsoft Word 10 (filtered)\">\n"
	"\n"
	"<style>\n"
	"<!--\n"
	" /* Style Definitions */\n"
	" p.MsoNormal, li.MsoNormal, div.MsoNormal\n"
	"	{margin:0cm;\n"
	"	margin-bottom:.0001pt;\n"
	"	font-size:12.0pt;\n"
	"	font-family:\"Times New Roman\";}\n"
	"a:link, span.MsoHyperlink\n"
	"	{color:blue;\n"
	"	text-decoration:underline;}\n"
	"a:visited, span.MsoHyperlinkFollowed\n"
	"	{color:purple;\n"
	"	text-decoration:underline;}\n"
	"span.Meililaad17\n"
	"	{font-family:Arial;\n"
	"	color:windowtext;}\n"
	"@page Section1\n"
	"	{size:595.3pt 841.9pt;\n"
	"	margin:70.85pt 70.85pt 70.85pt 70.85pt;}\n"
	"div.Section1\n"
	"	{page:Section1;}\n"
	"-->\n"
	"</style>\n"
	"\n"
	"</head>\n"
	"\n"
	"<body lang=3DET link=3Dblue vlink=3Dpurple>\n"
	"\n"
	"<div class=3DSection1>\n"
	"\n"
	"<p class=3DMsoNormal><font size=3D2 face=3DArial><span =\n"
	"style=3D'font-size:10.0pt;\n"
	"font-family:Arial'>Body of doc2</span></font></p>\n"
	"\n"
	"</div>\n"
	"\n"
	"</body>\n"
	"\n"
	"</html>\n"
	"\n"
	"------=_NextPart_D--\n"
	"\n"
	"------=_NextPart_C\n"
	"Content-Type: application/msword;\n"
	"	name=\"doc2.doc\"\n"
	"Content-Transfer-Encoding: base64\n"
	"Content-Disposition: attachment;\n"
	"	filename=\"doc2.doc\"\n"
	"\n"
	"0M8R4KGxGuEAAAAAAAAAAAAAAAAAAAAAPgADAP7/CQAGAAAAAAAAAAAAAAABAAAALgAAAAAAAAAA\n"
	"EAAAMAAAAAEAAAD+////AAAAAC8AAAD/////////////////////////////////////////////\n"
	"AAAAAwAAAAEAAAADAAAABQAAAAMAAACUGgoACwAAAAAAAAALAAAAAAAAAAsAAAAAAAAACwAAAAAA\n"
	"AAAeEAAAAQAAAAYAAABUZXN0MQAMEAAAAgAAAB4AAAAHAAAAVGlpdGVsAAMAAAABAAAAAAAAWAEA\n"
	"AAcAAAAAAAAAQAAAAAEAAAD0AAAAAAAAgPwAAAACAAAABAEAAAMAAAAMAQAABAAAACABAAAFAAAA\n"
	"SAEAAA==\n"
	"\n"
	"------=_NextPart_C--\n"
	"\n"
	"------=_NextPart_A\n"
	"Content-Type: message/rfc822\n"
	"Content-Transfer-Encoding: 7bit\n"
	"Content-Disposition: attachment\n"
	"\n"
	"From: \"try\" <try@test.kisise>\n"
	"To: <try@test.kisise>\n"
	"Subject: doc1\n"
	"Date: Wed, 4 Oct 2006 17:56:11 +0300\n"
	"Message-ID: <000001c6e7c5$3b28eff0$316fa8c0@RAIVO>\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed;\n"
	"	boundary=\"----=_NextPart_E\"\n"
	"X-Priority: 3 (Normal)\n"
	"X-MSMail-Priority: Normal\n"
	"X-Mailer: Microsoft Outlook, Build 10.0.6626\n"
	"X-DSPAM-Result: Innocent\n"
	"X-DSPAM-Processed: Thu Oct  5 03:19:15 2006\n"
	"X-DSPAM-Confidence: 0.9899\n"
	"X-DSPAM-Improbability: 1 in 9809 chance of being spam\n"
	"X-DSPAM-Probability: 0.0000\n"
	"X-DSPAM-Signature: 28,45244f83213031923176077\n"
	"X-DSPAM-Factors: 27,size+10, 0.01000,Content-Type*charset=\"us, 0.01000,Content-Type*charset=\"us, 0.01000,X-Mailer*Outlook+Build, 0.01000,Received*<try+test.kisise>, 0.01000,10, 0.01000,10, 0.01000,To*test.kisise>, 0.01000,of, 0.01000,of, 0.01000,<font, 0.01000,Received*ESMTP, 0.01000,X-MimeOLE*By, 0.01000,message, 0.01000,X-Priority*3+(Normal), 0.01000,X-MimeOLE*Produced, 0.01000,(filtered)\">+<font, 0.01000,X-Mailer*Build+10.0.6626, 0.01000,multi, 0.01000,Received*Oct, 0.01000,Content-Transfer-Encoding*quoted+printable, 0.01000,content=\"Microsoft, 0.01000,Received*by+test.kisise, 0.01000,<meta, 0.01000,10+(filtered)\">, 0.01000,charset=us+ascii\">, 0.01000,X-MimeOLE*Microsoft+MimeOLE, 0.01000\n"
	"X-DBMail-PhysMessage-ID: 103\n"
	"Importance: Normal\n"
	"X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.2962\n"
	"\n"
	"This is a multi-part message in MIME format.\n"
	"\n"
	"------=_NextPart_E\n"
	"Content-Type: multipart/alternative;\n"
	"	boundary=\"----=_NextPart_F\"\n"
	"\n"
	"\n"
	"------=_NextPart_F\n"
	"Content-Type: text/plain;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"Body of doc1\n"
	"\n"
	"\n"
	"------=_NextPart_F\n"
	"Content-Type: text/html;\n"
	"	charset=\"us-ascii\"\n"
	"Content-Transfer-Encoding: quoted-printable\n"
	"\n"
	"<html>\n"
	"\n"
	"<head>\n"
	"<META HTTP-EQUIV=3D\"Content-Type\" CONTENT=3D\"text/html; =\n"
	"charset=3Dus-ascii\">\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"<meta name=3DGenerator content=3D\"Microsoft Word 10 (filtered)\">\n"
	"\n"
	"<style>\n"
	"<!--\n"
	" /* Style Definitions */\n"
	" p.MsoNormal, li.MsoNormal, div.MsoNormal\n"
	"	{margin:0cm;\n"
	"	margin-bottom:.0001pt;\n"
	"	font-size:12.0pt;\n"
	"	font-family:\"Times New Roman\";}\n"
	"a:link, span.MsoHyperlink\n"
	"	{color:blue;\n"
	"	text-decoration:underline;}\n"
	"a:visited, span.MsoHyperlinkFollowed\n"
	"	{color:purple;\n"
	"	text-decoration:underline;}\n"
	"span.Meililaad17\n"
	"	{font-family:Arial;\n"
	"	color:windowtext;}\n"
	"@page Section1\n"
	"	{size:595.3pt 841.9pt;\n"
	"	margin:70.85pt 70.85pt 70.85pt 70.85pt;}\n"
	"div.Section1\n"
	"	{page:Section1;}\n"
	"-->\n"
	"</style>\n"
	"\n"
	"</head>\n"
	"\n"
	"<body lang=3DET link=3Dblue vlink=3Dpurple>\n"
	"\n"
	"<div class=3DSection1>\n"
	"\n"
	"<p class=3DMsoNormal><font size=3D2 face=3DArial><span =\n"
	"style=3D'font-size:10.0pt;\n"
	"font-family:Arial'>Body of doc1</span></font></p>\n"
	"\n"
	"</div>\n"
	"\n"
	"</body>\n"
	"\n"
	"</html>\n"
	"\n"
	"------=_NextPart_F--\n"
	"\n"
	"------=_NextPart_E\n"
	"Content-Type: application/msword;\n"
	"	name=\"doc1.doc\"\n"
	"Content-Transfer-Encoding: base64\n"
	"Content-Disposition: attachment;\n"
	"	filename=\"doc1.doc\"\n"
	"\n"
	"0M8R4KGxGuEAAAAAAAAAAAAAAAAAAAAAPgADAP7/CQAGAAAAAAAAAAAAAAABAAAALgAAAAAAAAAA\n"
	"wAAAAA0AAADIAAAADAAAANoAAAACAAAA6QQAAB4AAAAQAAAAS3JlZGlpZGlpbmZvIEFTAAMAAAAB\n"
	"AAAAAwAAAAEAAAADAAAABQAAAAMAAACUGgoACwAAAAAAAAALAAAAAAAAAAsAAAAAAAAACwAAAAAA\n"
	"AAAeEAAAAQAAAAYAAABUZXN0MQAMEAAAAgAAAB4AAAAHAAAAVGlpdGVsAAMAAAABAAAAAAAAWAEA\n"
	"AAcAAAAAAAAAQAAAAAEAAAD0AAAAAAAAgPwAAAACAAAABAEAAAMAAAAMAQAABAAAACABAAAFAAAA\n"
	"SAEAAA==\n"
	"\n"
	"------=_NextPart_E--\n"
	"\n"
	"------=_NextPart_A--\n"
	"\n";

char * broken_message = "From paul@nfg.nl Tue Oct 11 13:06:24 2005\n"
	"Subject: [Fwd: test multipart 3]\n"
	"From: Paul J Stevens <paul@nfg.nl>\n"
	"To: foo@bar\n"
	"X-Evolution-Account: 1073466346.10467.0@tenkai\n"
	"X-Evolution-Format: text/plain\n"
	"X-Mailer: Evolution 2.2.3\n"
	"Date: Tue, 08 Nov 2005 14:49:40 +0100\n"
	"Message-Id: <1131457780.16526.6.camel@tenkai>\n"
	"X-DBMail-PhysMessage-ID: 722200\n"
	"MIME-Version: 1.0\n"
	"Content-Type: multipart/mixed; boundary=\"=-i5BOOWGh5HearcweMC39\"\n"
	"\n"
	"\n"
	"--=-i5BOOWGh5HearcweMC39\n"
	"Content-Type: multipart/alternative; boundary=\"=-GVq83nniGaGqAWLlt+2v\"\n"
	"\n"
	"\n"
	"--=-GVq83nniGaGqAWLlt+2v\n"
	"Content-Type: text/plain\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"\n"
	"-- \n"
	"Paul J Stevens <paul@nfg.nl>\n"
	"NFG Net Facilities Group BV\n"
	"\n"
	"--=-GVq83nniGaGqAWLlt+2v\n"
	"Content-Type: text/html; charset=utf-8\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\n"
	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 TRANSITIONAL//EN\">\n"
	"<HTML>\n"
	"<HEAD>\n"
	"  <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; CHARSET=UTF-8\">\n"
	"    <META NAME=\"GENERATOR\" CONTENT=\"GtkHTML/3.6.2\">\n"
	"    </HEAD>\n"
	"    <BODY>\n"
	"    <!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"orig\" value=\"0\">--><!--+GtkHTML:<DATA "
	"class=\"ClueFlow\" key=\"signature\" value=\"0\">--><!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"signature_name\" value=\"uid:1105006996..1825..0@tenkai\">--><BR>\n"
	"    <!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"signature\" value=\"1\">--><TABLE CELLSPACING=\"0\" CELLPADDING=\"0\" WIDTH=\"100%\">\n"
	"    <TR>\n"
	"    <TD>\n"
	"    <!--+GtkHTML:<DATA class=\"ClueFlow\" clear=\"orig\">--><!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"signature\" value=\"1\">-->-- <BR>\n"
	"    <!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"signature\" value=\"1\">-->Paul J Stevens &lt;<A HREF=\"mailto:paul@nfg.nl\">paul@nfg.nl</A>&gt;<BR>\n"
	"    <!--+GtkHTML:<DATA class=\"ClueFlow\" key=\"signature\" value=\"1\">-->NFG Net Facilities Group BV\n"
	"    </TD>\n"
	"    </TR>\n"
	"    </TABLE>\n"
	"    </BODY>\n"
	"    </HTML>\n"
	"\n"
	"    --=-GVq83nniGaGqAWLlt+2v--\n"
	"\n"
	"    --=-i5BOOWGh5HearcweMC39\n"
	"    Content-Disposition: inline\n"
	"    Content-Description: Forwarded message - test multipart 3\n"
	"    Content-Type: message/rfc822\n"
	"\n";


char * broken_message2 = "Received: from localhost (localhost [127.0.0.1])\n"
	"	by mail.xxxxxx.nl (Postfix) with ESMTP id 0DE53406A9\n"
	"	for <sales@xyxyxyxyx.com>; Sat,  1 Sep 2007 16:45:26 +0200 (CEST)\n"
	"X-Virus-Scanned: Debian amavisd-new at xxxxxx.nl\n"
	"Received: from mail.xxxxxx.nl ([127.0.0.1])\n"
	"	by localhost (mail.xxxxxx.nl [127.0.0.1]) (amavisd-new, port 10024)\n"
	"	with ESMTP id q2xB9UoFdPnD for <sales@xyxyxyxyx.com>;\n"
	"	Sat,  1 Sep 2007 16:45:23 +0200 (CEST)\n"
	"Received: from [121.181.42.214] (unknown [121.181.42.214])\n"
	"	by mail.xxxxxx.nl (Postfix) with ESMTP id E80C1406A4\n"
	"	for <sales@xyxyxyxyx.com>; Sat,  1 Sep 2007 16:45:22 +0200 (CEST)\n"
	"Received: from [121.181.42.214] by mail9.intake.net; Sat, 32 Aug 2007 23:51:42 +0900\n"
	"Message-ID: <0107ffa4$0107fe78$d62ab579@einar>\n"
	"From: \"Rena Katz\" <einar@spiritspear.com>\n"
	"To: <sales@xyxyxyxyx.com>\n"
	"Subject: adobe acrobat v8.0 $79\n"
	"Date: Sat, 32 Aug 2007 23:51:42 +0900\n"
	"MIME-Version: 1.0\n"
	"Content-Type: text/plain;\n"
	"	format=flowed;\n"
	"	charset=\"Windows-1252\";\n"
	"	reply-type=original\n"
	"Content-Transfer-Encoding: 7bit\n"
	"X-Priority: 3\n"
	"X-MSMail-Priority: Normal\n"
	"X-Mailer: Microsoft Outlook Express 6.00.2900.2180\n"
	"X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.2180\n"
	"\n"
	"ADOBE PHOTOSHOP CS3 EXTENDED Retail price - $999 Save: $909.05\n"
	"http://www.cndomenchegi.cn\n"
	"\n";

