#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TEATLIBNEW_LIB)
#  define TEATLIBNEW_EXPORT Q_DECL_EXPORT
# else
#  define TEATLIBNEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define TEATLIBNEW_EXPORT
#endif
