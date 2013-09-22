#!/usr/bin/php
<?php

$host = ($_SERVER['argc'] == 2) ? $_SERVER['argv'][1] : 'localhost';
$port = 11139;
if (($sock = fsockopen($host, $port)) === false) {
	fprintf(STDOUT, "Unable to connect to the server.\n");
	exit(1);
}
while (true) {
	print('> ');
	$input = input();
	$cmd = substr($input, 0, 4);
	$cmd = trim($cmd);
	$input = substr($input, 4);
	$input = trim($input);
	if (!strcasecmp($cmd, 'help'))
		command_help();
	else if (!strcasecmp($cmd, 'put'))
		command_put($sock, $input);
	else if (!strcasecmp($cmd, 'del'))
		command_del($sock, $input);
	else if (!strcasecmp($cmd, 'get'))
		command_get($sock, $input);
	else if (!strcasecmp($cmd, 'quit'))
		exit();
	print("\n");
}

/**
 * Process a GET command.
 * @param	$sock	stream	The connection socket.
 * @param	$input	string	The command string.
 */
function command_get($sock, $input) {
	// extract key and value
	if ($input[0] != '"' || ($pos = strpos($input, '"', 1)) === false) {
		print("\tSyntax error\n");
		return;
	}
	$key = substr($input, 1, $pos);
	// send the command
	$buffer = chr(2) . chr(mb_strlen($key, 'ascii')) . $key;
	fwrite($sock, $buffer);
	$response = fread($sock, 1);
	if (ord($response) != 1) {
		print("\tERROR\n");
		return;
	}
print("reponse ok\n");
	$response = fread($sock, 4);
	$length = unpack('Nint', $response);
	$length = $length['int'];
print("length : '$length'\n");
	$value = null;
	if ($length > 0) {
		$value = fread($sock, $length);
		print("$value\n");
	}
}
/**
 * Process a DEL command.
 * @param	$sock	stream	The connection socket.
 * @param	$input	string	The command string.
 */
function command_del($sock, $input) {
	// extract key and value
	if ($input[0] != '"' || ($pos = strpos($input, '"', 1)) === false) {
		print("\tSyntax error\n");
		return;
	}
	$key = substr($input, 1, $pos);
	// send the command
	$buffer = chr(2) . chr(mb_strlen($key, 'ascii')) . $key;
	fwrite($sock, $buffer);
	$response = fread($sock, 1);
	if (ord($response) != 1)
		print("\tERROR\n");
	else
		print("\tOK\n");
}
/**
 * Process a PUT command.
 * @param	$sock	stream	The connection socket.
 * @param	$input	string	The command string.
 */
function command_put($sock, $input) {
	// extract key and value
	if ($input[0] != '"' || ($pos = strpos($input, '"', 1)) === false) {
		print("\tSyntax error\n");
		return;
	}
	$key = substr($input, 1, $pos);
	$input = substr($input, $pos + 1);
	$input = trim($input);
	if ($input[0] != '"' || substr($input, -1) != '"') {
		print("\tSyntax error\n");
		return;
	}
	$value = substr($input, 1, -1);
	// send the command
	$buffer = chr(1) . chr(mb_strlen($key, 'ascii')) . $key .
		  pack('N', mb_strlen($value, 'ascii')) . $value;
	fwrite($sock, $buffer);
	$response = fread($sock, 1);
	if (ord($response) != 1)
		print("\tERROR\n");
	else
		print("\tOK\n");
}
/** Show the help. */
function command_help() {
	print("\tput \"key\" \"value\"\n");
	print("\tdel \"key\"\n");
	print("\tget \"key\"\n");
	print("\tquit\n");
}
/**
 * Get user input.
 * @return	The user input.
 */
function input() {
	$stdin = fopen('php://stdin', 'r');
	$s = trim(fgets($stdin));
	return ($s);
}
