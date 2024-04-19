
#ifdef __cplusplus
  class ICommunication
  {
  public:
	virtual bool readMsg()=0;
	virtual bool writeMsg()=0;
	virtual void init()=0;
  };
#endif
