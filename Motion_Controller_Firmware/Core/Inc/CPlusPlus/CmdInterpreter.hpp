#include "main.h"

#ifdef __cplusplus
  class CmdInterpreter
  {
  public:
	  CmdInterpreter();
	  void cmd();
	  double getPos();
	  void setParamsIfNeeded();

  private:
	  bool m_NeedsMotorPos;

  };
#endif




#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STDC__) || defined(__cplusplus)
  //extern void c_function(CmdInterpreter*);   /* ANSI C prototypes */
  //extern CmdInterpreter* cplusplus_callback_function(CmdInterpreter*);
  //extern Fred* cplusplus_callback_function(Fred*);
//#else
  //extern void c_function();        /* K&R style */
  //extern Fred* cplusplus_callback_function();
#endif
#ifdef __cplusplus
}
#endif


//#define EXTERNC
//#endif
//EXTERNC void cmd();



