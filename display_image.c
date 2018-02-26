int pow(int x, int y){ // power function is needed since we don't have libraries
	int i;
	int ans = 1;
	for(i = 0; i < y; i++){
		ans = ans * x;
	}
	return ans;
}
void display_image(int y, const uint8_t *data) {
	int x = 0;
	int i, j;
	int a[32];	//transfers "marked" pixels from one page to another
	
	for(j = 0; j<32; j++){	// initialize the array to only zeros
		a[j] = 0;
	}
	
	int pg = y/8;	// the number of pages the object moves
	y = y % 8;		// number of lines in addition to pages that the object moves
	
	for(i = 0; i < 4 - y/8; i++) { // changed the bounds to prevent overflow
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i + pg); // i + pg is the page we're moving to
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		// checks whether we have to transfer any pixels from one page to another
		// and sends the right data to the buffer
		for(j = 0; j < 32; j++){
			if(data[i*32 + j] < pow(2, 8- y)){ // if pg is 1 we check if the number is less than 128, if pg is 2 we check 64...
				spi_send_recv( pow(2, y) *  data[i *32 + j] + a[j]); 
				a[j] = 0;
			}
			else{
				spi_send_recv( pow(2, y)  * (data[ i*32 + j] - 128) + a[j]); // if the number is greater than the number we check, we need to change page
				a[j] = (data[i*32 + j] >> (8 - y));	
			}
		}	
	}
}
