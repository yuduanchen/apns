
###Summary

APNS - Apple Push Notification Service 's php extension

###Maintainers
chenyuduan(lead)

###License
PHP


## Install

## Release

PHP Version: PHP 5.3.5 or newer



```
$/path/to/phpize

$./configure

$make && make install


```

## Documentation



### The apns class
```c
apns{
    /* variable */
    public static string $passphrase = "" ;//ssl Password
    public static string $certificate = "" ;//ssl authorization file
    public static string $gateway = "gateway.push.apple.com:2195" ;//IOS APNS URL
    /* methods */

    /***
     * Connection apple apns service
     */
    public static connect();

    /***
     * send notification
     * @param string $token
     * @param string $message
     */
    public static send(string $token,string $message);
}

```
### Return Values

1 success

0 failure

## Exception

 SSL_INIT_FAIL  -------- Initialization failed (初始化失败)

 CERTIFICATE_NOT_FOUNT -------- Can't find the authorization file (找不到授权文件)

 CERTIFICATE_PASSWORD_FAIL -------- Password mistake (口令不正确)

 SSL_CONNECT_FAIL -------- Connection failed (连接失败)


### Demo

```php
<?php
apns::$passphrase ="xxxxx";   //password
apns::$certificate="./ck.pem"; //certificate file
try{
    apns::connect();
    $token=str_replace(" ","","811111111111111111111111111111");
    echo apns::send($token,"{\"aps\":{\"alert\":\"test1\",\"badge\":1}}");
    echo apns::send($token,"{\"aps\":{\"alert\":\"test2\",\"badge\":1}}");
}catch(exception $e){
    echo $e->getMessage();
}
?>
```
### Application

蜗牛帮 http://www.woniubang.com


### more

Site: http://www.cydphp.cn





