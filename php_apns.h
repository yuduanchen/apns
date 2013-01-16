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

#ifndef PHP_APNS_H
#define PHP_APNS_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#ifdef HAVE_APNS
#define PHP_APNS_VERSION "1.0"


#include <php_ini.h>
#include <SAPI.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEVICE_BINARY_SIZE 32
#define MAXPAYLOAD_SIZE 256 ;


#ifdef  __cplusplus
} // extern "C"
#endif
#ifdef  __cplusplus
extern "C" {
#endif

extern zend_module_entry apns_module_entry;
#define phpext_apns_ptr &apns_module_entry

#ifdef PHP_WIN32
#define PHP_APNS_API __declspec(dllexport)
#else
#define PHP_APNS_API
#endif

PHP_MINIT_FUNCTION(apns);
PHP_MSHUTDOWN_FUNCTION(apns);
PHP_RINIT_FUNCTION(apns);
PHP_RSHUTDOWN_FUNCTION(apns);
PHP_MINFO_FUNCTION(apns);

#ifdef ZTS
#include "TSRM.h"
#endif

#define FREE_RESOURCE(resource) zend_list_delete(Z_LVAL_P(resource))

#define PROP_GET_LONG(name)    Z_LVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_LONG(name, l) zend_update_property_long(_this_ce, _this_zval, #name, strlen(#name), l TSRMLS_CC)

#define PROP_GET_DOUBLE(name)    Z_DVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_DOUBLE(name, d) zend_update_property_double(_this_ce, _this_zval, #name, strlen(#name), d TSRMLS_CC)

#define PROP_GET_STRING(name)    Z_STRVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_GET_STRLEN(name)    Z_STRLEN_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_STRING(name, s) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s TSRMLS_CC)
#define PROP_SET_STRINGL(name, s, l) zend_update_property_stringl(_this_ce, _this_zval, #name, strlen(#name), s, l TSRMLS_CC)

ZEND_BEGIN_MODULE_GLOBALS(apns)
ZEND_END_MODULE_GLOBALS(apns)

#ifdef ZTS
#define APNS_G(v) TSRMG(apns_globals_id, zend_apns_globals *, v)
#else
#define APNS_G(v) (apns_globals.v)
#endif


PHP_METHOD(apns, connect);
PHP_METHOD(apns, send);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(apns__send_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
  ZEND_ARG_INFO(0, x)
  ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define apns__send_args NULL
#endif

#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(apns__connect_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define apns__send_args NULL
#endif

#ifdef  __cplusplus
} // extern "C"
#endif

#endif /* PHP_HAVE_APNS */

#endif /* PHP_APNS_H */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
