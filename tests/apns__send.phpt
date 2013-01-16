--TEST--
apns::send() member function
--SKIPIF--
<?php 

if(!extension_loaded('apns')) die('skip ');

 ?>
--FILE--
<?php
echo 'OK'; // no test case for this function yet
?>
--EXPECT--
OK