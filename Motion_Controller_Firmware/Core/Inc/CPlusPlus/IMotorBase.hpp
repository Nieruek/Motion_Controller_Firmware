#ifdef __cplusplus
  class IMotorBase
  {
  public:
	//virtual void MotorControl()=0;
	  virtual void CurrentRegulation()=0;
	  virtual void PositionRegulation()=0;
	  virtual void TrajectoryGenerator()=0;
	  virtual void setPWMValue()=0;
	  virtual void OutputMatrix()=0;
  };
#endif
