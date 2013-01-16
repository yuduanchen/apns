dnl
dnl $ Id: $
dnl

PHP_ARG_ENABLE(apns, whether to enable apns functions,
[  --enable-apns         Enable apns support])

if test "$PHP_APNS" != "no"; then
  export OLD_CPPFLAGS="$CPPFLAGS"
  export CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_APNS"

  AC_MSG_CHECKING(PHP version)
  AC_TRY_COMPILE([#include <php_version.h>], [
#if PHP_VERSION_ID < 50000
#error  this extension requires at least PHP version 5.0.0
#endif
],
[AC_MSG_RESULT(ok)],
[AC_MSG_ERROR([need at least PHP 5.0.0])])

  export CPPFLAGS="$OLD_CPPFLAGS"


  PHP_SUBST(APNS_SHARED_LIBADD)
  AC_DEFINE(HAVE_APNS, 1, [ ])

  PHP_NEW_EXTENSION(apns, apns.c , $ext_shared)

fi

