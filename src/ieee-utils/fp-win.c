#include <float.h>

#include <config.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_errno.h>

const char *fp_env_string = "round-to-nearest,double-precision,mask-all";

int
gsl_ieee_set_mode (int precision, int rounding, int exception_mask)
{
	unsigned int old, mode = _DN_SAVE, mask = _MCW_DN | _MCW_RC | _MCW_EM;

	switch(precision)
    {
    case GSL_IEEE_SINGLE_PRECISION:		mode |= _PC_24; break;
    case GSL_IEEE_EXTENDED_PRECISION:	mode |= _PC_64; break;
    case GSL_IEEE_DOUBLE_PRECISION:
    default:							mode |= _PC_53;
	}
	mask |= _MCW_PC;

	switch(rounding)
    {
    case GSL_IEEE_ROUND_DOWN:			mode |= _RC_DOWN; break;
    case GSL_IEEE_ROUND_UP:				mode |= _RC_UP;   break;
    case GSL_IEEE_ROUND_TO_ZERO:		mode |= _RC_CHOP; break;
    case GSL_IEEE_ROUND_TO_NEAREST:
    default:							mode |= _RC_NEAR;
    }

	if(exception_mask & GSL_IEEE_MASK_INVALID)
		mode |= _EM_INVALID;
	if(exception_mask & GSL_IEEE_MASK_DENORMALIZED)
		mode |= _EM_DENORMAL;
	if(exception_mask & GSL_IEEE_MASK_DIVISION_BY_ZERO)
		mode |= _EM_ZERODIVIDE;
	if(exception_mask & GSL_IEEE_MASK_OVERFLOW)
		mode |= _EM_OVERFLOW;
	if(exception_mask & GSL_IEEE_MASK_UNDERFLOW)
		mode |= _EM_UNDERFLOW;
	if(exception_mask & GSL_IEEE_TRAP_INEXACT)
		mode &= ~_EM_INEXACT;
	else
		mode |= _EM_INEXACT;

	_controlfp_s( &old, mode, mask);
	return GSL_SUCCESS;
}