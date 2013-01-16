apns
====

为  APNS(Apple Push Notification Service) 消息推送 所开发的 PHP扩展

## 安装APNS

## 系统要求

>  php5.3.5 以上


```
$/path/to/phpize

$./configure

$make && make install


```

## 文档



###类摘要
```c
apns{
    /* 变量 */
    public static string $passphrase = "" ;//密码
    public static string $certificate = "" ;//ssh授权文件
    public static string $gateway = "gateway.push.apple.com:2195" ;//IOS APNS 服务器推送接口
    /* 方法 */

    /***
     * 连接apple apns服务
     */
    public static connect();

    /***
     * 推送信息
     * @param string $token
     * @param string $message
     */
    public static send(string $token,string $message);
}

```
### 返回值

1 成功

0 失败

### 异常类型

 SSL_INIT_FAIL 初始化失败

 CERTIFICATE_NOT_FOUNT 找不到授权文件

 CERTIFICATE_PASSWORD_FAIL 口令不正确

 SSL_CONNECT_FAIL 连接失败


### 例子

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
### 应用案例

蜗牛帮 http://www.woniubang.com


### 更多问题

详解 http://www.cydphp.cn





