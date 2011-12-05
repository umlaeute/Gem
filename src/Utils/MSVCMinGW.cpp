/* stubs for compiling with MinGW
 *
 * seems to be needed since MinGW provides ISO-C99 compatible printf() implementation
 * which older MSVCrt does not provide yet
 */


extern "C" int _get_output_format( void ) { return 1; }
