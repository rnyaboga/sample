//*****************************************************************************
//
//! This function wirtes nBytes from outBuffer to nand flash. max value is 256
//!
//! \param nBytes - Number of bytes to write
//!
//! \return None
//!
//! \var memSize - Number of bytes written
//! \def PAGE_SIZE - Size of current page
//! \def MEMSIZE - Total memory size
//! \var memAddr - Current memory address in a die
//! \var uActiveChipsID - Current nand flash die
//! \var state - state of operation
//! \var uNumOfChipsInstalled - Available nand flash dies
//! \var uNumOfChipsInstalled - Available nand flash dies
//!
//*****************************************************************************
void writeBytesNonBlocking2(unsigned short nBytes) {
	if ((memSize + nBytes) > PAGE_SIZE) {				//writes will exceed page
		state = 0;										//goto to next page
	}
	if ((memAddr + PAGE_SIZE) > (MEMSIZE)){				//last page of current die reached
		if(uActiveChipsID + 1 > uNumOfChipsInstalled){	//last Die reached
			return;
		}
		uActiveChipsID++;								//move to next die
		memAddr = 0;									//start page at zero
		state = 1;
	}
	switch (state) {
	case 0:												//close page
		closeWriteBytesNonBlocking();					//close this page
		memAddr += PAGE_SIZE;							//only increment when closed
		memSize = 0;
		state = 1;
														//no break just roll to next case
	case 1:												//prepate to write
		openWriteBytesNonBlocking();
		state = 2;
														//no break just roll to next case
	case 2:												//write bytes
		writeBytesCycle(nBytes);						//write bytes from buffer
		memSize += nBytes;
		if (memSize >= PAGE_SIZE) {
			closeWriteBytesNonBlocking();				//close this page
			memAddr += PAGE_SIZE;						//only increment when closed
			memSize = 0;
			state = 1;
		}
		break;
	default:
		state = 0;
		break;
	}
}

