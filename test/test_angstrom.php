#!/usr/bin/php
<?php

const ANGSTROM_HOST = 'localhost';
const ANGSTROM_PORT = 11139;

$nbrLoops = 1;
$dataList = array(
	'key1'			=> 'aaa',
	//'second key'		=> 'bbb',
	//'another longer key'	=> 'ccc'
);

for ($i = 0; $i < $nbrLoops; $i++) {
	foreach ($dataList as $key => $data) {
		$sock = fsockopen(ANGSTROM_HOST, ANGSTROM_PORT);
		commandPut($sock, "$key-$i", $data);
		fclose($sock);
	}
}
$sock = fsockopen(ANGSTROM_HOST, ANGSTROM_PORT);
for ($i = 0; $i < $nbrLoops; $i++) {
	foreach ($dataList as $key => $data) {
		$value = commandGet($sock, "$key-$i");
		print("GET '$key' => '$value' ($data)\n");
		if ($value != $data)
			print("GET error for key '$key-$i'\n");
	}
}
fclose($sock);

/**
 * Send data to AngstromDB server.
 * @param	stream	$sock	Socket descriptor.
 * @param	string	$key	Key of the data.
 * @param	string	$data	Data to send.
 */
function commandPut($sock, $key, $data) {
	$buffer = chr(1) . chr(mb_strlen($key, 'ascii')) . $key .
		  pack('N', mb_strlen($data, 'ascii')) . $data;
	fwrite($sock, $buffer);
	$response = fread($sock, 4096);
	if (ord($response) != 1)
		print("PUT error\n");
}
/**
 * Fetch data from FineDB server.
 * @param	stream	$sock	Socket descriptor.
 * @param	string	$key	Key of the data.
 */
function commandGet($sock, $key) {
	$buffer = chr(3) . chr(mb_strlen($key, 'ascii')) . $key;
	fwrite($sock, $buffer);
	$response = fread($sock, 1);
	if (ord($response) != 1) {
		print("GET error\n");
		return;
	}
	$response = fread($sock, 4);
	$length = unpack('Nint', $response);
	$length = $length['int'];
	$value = null;
	if ($length > 0)
		$value = fread($sock, $length);
	return ($value);
}


