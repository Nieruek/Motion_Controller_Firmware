##Classic CAN/CANFD: 
*aktivieren (siehe unten)
*Initialisierung vom CAN/CANFD
	FDCAN_TxHeaderTypeDef   TxHeader;
	FDCAN_RxHeaderTypeDef   RxHeader;
	uint8_t               TxData[12];
	uint8_t               RxData[12];
	int indx = 0;
	
	if(HAL_FDCAN_Start(&hfdcan1)!= HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

  	TxHeader.Identifier = 0x11;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_12;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_FD_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

*Senden: 
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
	 {
	  Error_Handler();
	 }
	 
*Empfangen mit Interrupt etc. möglich