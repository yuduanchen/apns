/*
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license,       |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_0.txt.                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: chenyuduan <thisduan@gmail.com>                             |
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */

#include "php_apns.h"

#if HAVE_APNS

SSL *ssl;

/* {{{ Class definitions */

/* {{{ Class apns */

static zend_class_entry * apns_ce_ptr = NULL;




typedef int  bool;
static bool send_payload(SSL *sslPtr, char *deviceTokenBinary, char *payloadBuff, size_t payloadLength)
{
    bool rtn = 0;
    if (sslPtr && deviceTokenBinary && payloadBuff && payloadLength)
    {
        uint8_t command = 1; /* command number */
        int l=sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint16_t) +
              DEVICE_BINARY_SIZE + sizeof(uint16_t) + MAXPAYLOAD_SIZE;
        char binaryMessageBuff[l];
        /* message format is, |COMMAND|ID|EXPIRY|TOKENLEN|TOKEN|PAYLOADLEN|PAYLOAD| */
        char *binaryMessagePt = binaryMessageBuff;
        uint32_t whicheverOrderIWantToGetBackInAErrorResponse_ID = 1234;
        uint32_t networkOrderExpiryEpochUTC = htonl(time(NULL)+86400); // expire message if not delivered in 1 day
        uint16_t networkOrderTokenLength = htons(DEVICE_BINARY_SIZE);
        uint16_t networkOrderPayloadLength = htons(payloadLength);

        /* command */
        *binaryMessagePt++ = command;

        /* provider preference ordered ID */
        memcpy(binaryMessagePt, &whicheverOrderIWantToGetBackInAErrorResponse_ID, sizeof(uint32_t));
        binaryMessagePt += sizeof(uint32_t);

        /* expiry date network order */
        memcpy(binaryMessagePt, &networkOrderExpiryEpochUTC, sizeof(uint32_t));
        binaryMessagePt += sizeof(uint32_t);

        /* token length network order */
        memcpy(binaryMessagePt, &networkOrderTokenLength, sizeof(uint16_t));
        binaryMessagePt += sizeof(uint16_t);

        /* device token */
        memcpy(binaryMessagePt, deviceTokenBinary, DEVICE_BINARY_SIZE);
        binaryMessagePt += DEVICE_BINARY_SIZE;

        /* payload length network order */
        memcpy(binaryMessagePt, &networkOrderPayloadLength, sizeof(uint16_t));
        binaryMessagePt += sizeof(uint16_t);

        /* payload */
        memcpy(binaryMessagePt, payloadBuff, payloadLength);
        binaryMessagePt += payloadLength;
        if (SSL_write(sslPtr, binaryMessageBuff, (binaryMessagePt - binaryMessageBuff)) > 0)
            rtn =1;
    }
    return rtn;
}

void string_to_bytes( const char *token, char *bytes)
{
    int val;
    while (*token)
    {
        sscanf(token, "%2x", &val);
        *(bytes++) = (char)val;
        token += 2;
    }
}

/* {{{ proto void send(string x)
   */
PHP_METHOD(apns, connect)
{
    zend_class_entry * _this_ce;

    zval * _this_zval = NULL;
    const char * x = NULL;
    int x_len = 0;

    const char * y = NULL;
    int y_len = 0;


    BIO *bio;



    SSL_CTX  *ssl_ctx;

    SSL_library_init();

    char *certificateDir,*passphraseStr,*gatewayUrl;

    zval *certificate = zend_read_static_property(apns_ce_ptr, ZEND_STRL("certificate"), sizeof ("certificate"));
    spprintf(&certificateDir, 0, "%s", Z_STRVAL_P(certificate));
    zval *passphrase = zend_read_static_property(apns_ce_ptr, ZEND_STRL("passphrase"), sizeof ("passphrase"));
    spprintf(&passphraseStr, 0, "%s", Z_STRVAL_P(passphrase));
    zval *gateway = zend_read_static_property(apns_ce_ptr, ZEND_STRL("gateway"), sizeof ("gateway"));
    spprintf(&gatewayUrl, 0, "%s", Z_STRVAL_P(gateway));


    ssl_ctx = SSL_CTX_new(SSLv23_method());

    if (ssl_ctx == NULL)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "SSL_INIT_FAIL",  2 TSRMLS_CC);
        return ;
    }

    if (SSL_CTX_use_certificate_chain_file(ssl_ctx, certificateDir) != 1)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "CERTIFICATE_NOT_FOUNT",  2 TSRMLS_CC);
        return ;
    }



    SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx,passphraseStr);

    if (SSL_CTX_use_PrivateKey_file(ssl_ctx, certificateDir, SSL_FILETYPE_PEM) != 1)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "CERTIFICATE_PASSWORD_FAIL",  2 TSRMLS_CC);
        return ;
    }

    bio = BIO_new_connect(gatewayUrl);

    if(!bio)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "SSL_CONNECT_FAIL",  2 TSRMLS_CC);
        return ;
    }


    if (BIO_do_connect(bio) <= 0)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "SSL_CONNECT_FAIL",  2 TSRMLS_CC);
        return ;
    }

    if (!(ssl = SSL_new(ssl_ctx)))
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "SSL_CONNECT_FAIL",  2 TSRMLS_CC);
        return ;
    }

    SSL_set_bio(ssl, bio, bio);
    if (SSL_connect(ssl) <= 0)
    {
        zend_throw_exception_ex(apns_ce_ptr, 0 TSRMLS_CC, "SSL_CONNECT_FAIL",  2 TSRMLS_CC);
        return ;
    }

    RETURN_TRUE;
}
/* }}} send */

/* {{{ proto void send(string x)
   */
PHP_METHOD(apns, send)
{
    zend_class_entry * _this_ce;

    zval * _this_zval = NULL;
    const char * x = NULL;
    int x_len = 0;

    const char * y = NULL;
    int y_len = 0;



    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "ss",   &x, &x_len,&y,&y_len) == FAILURE)
    {
        return;
    }



    char tokenBytes[32];
    string_to_bytes(x, tokenBytes);
    if(send_payload(ssl, tokenBytes, (char*)y,strlen( (char*)y))>=1)
    {
        RETURN_TRUE;
    }
    else
    {
        RETVAL_FALSE
    }


}
/* }}} send */


static zend_function_entry apns_methods[] =
{

    PHP_ME(apns, connect, apns__connect_args, /**/ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(apns, send, apns__send_args, /**/ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    {
        NULL, NULL, NULL
    }
};

/* }}} Methods */

static void class_init_apns(void)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "apns", apns_methods);
    apns_ce_ptr = zend_register_internal_class(&ce);

    zend_declare_property_string(apns_ce_ptr, "certificate", strlen("certificate"), "null", ZEND_ACC_STATIC TSRMLS_CC);
    zend_declare_property_string(apns_ce_ptr, "gateway", strlen("gateway"), "gateway.push.apple.com:2195", ZEND_ACC_STATIC TSRMLS_CC);
    zend_declare_property_string(apns_ce_ptr, "passphrase", strlen("passphrase"), "", ZEND_ACC_STATIC TSRMLS_CC);

}

/* }}} Class apns */

/* }}} Class definitions*/

/* {{{ apns_functions[] */
function_entry apns_functions[] =
{

};
/* }}} */


/* {{{ apns_module_entry
 */
zend_module_entry apns_module_entry =
{
    STANDARD_MODULE_HEADER,
    "apns",
    apns_functions,
    PHP_MINIT(apns),     /* Replace with NULL if there is nothing to do at php startup   */
    PHP_MSHUTDOWN(apns), /* Replace with NULL if there is nothing to do at php shutdown  */
    PHP_RINIT(apns),     /* Replace with NULL if there is nothing to do at request start */
    PHP_RSHUTDOWN(apns), /* Replace with NULL if there is nothing to do at request end   */
    PHP_MINFO(apns),
    PHP_APNS_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_APNS
ZEND_GET_MODULE(apns)
#endif


/* {{{ globals and ini entries */
ZEND_DECLARE_MODULE_GLOBALS(apns)


#ifndef ZEND_ENGINE_2
#define OnUpdateLong OnUpdateInt
#endif
PHP_INI_BEGIN()
//STD_PHP_INI_ENTRY("apns.gateway", "ssl://gateway.push.apple.com:2195 ", PHP_INI_ALL, OnUpdateLong, gateway, zend_apns_globals, apns_globals)
PHP_INI_END()

static void php_apns_init_globals(zend_apns_globals *apns_globals)
{

}

static void php_apns_shutdown_globals(zend_apns_globals *apns_globals)
{
}/* }}} */


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(apns)
{
    ZEND_INIT_MODULE_GLOBALS(apns, php_apns_init_globals, php_apns_shutdown_globals)
    REGISTER_INI_ENTRIES();
    REGISTER_LONG_CONSTANT("apns_version", 1.0, CONST_PERSISTENT | CONST_CS);
    class_init_apns();

    /* add your stuff here */

    return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(apns)
{
    UNREGISTER_INI_ENTRIES();

    /* add your stuff here */

    return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(apns)
{
    /* add your stuff here */

    return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(apns)
{
    /* add your stuff here */

    return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(apns)
{

    php_info_print_table_start();
    php_info_print_table_row(2, "Version",PHP_APNS_VERSION " (stable)");
    php_info_print_table_row(2, "Default gateway","gateway.push.apple.com:2195");
    php_info_print_table_end();
    php_info_print_table_start();
    php_info_print_table_row(2, "Authors", "chenyuduan");
    php_info_print_table_row(2, "Supports", "http://www.cydphp.cn/php_apns");
    php_info_print_table_end();
    /* add your stuff here */

    DISPLAY_INI_ENTRIES();
}
/* }}} */




#endif /* HAVE_APNS */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
