#ifndef _MODEL_CHARNAMES_H_
#define _MODEL_CHARNAMES_H_

namespace charnames {
//				code	value	escape	description
	const char nul	=	0x00;	// \0	Null character
	const char soh	=	0x01;	//		Start of Header
	const char stx	=	0x02;	//		Start of Text
	const char etx	=	0x03;	//		End of Text
	const char eot	=	0x04;	//		End of Transmission
	const char enq	=	0x05;	//		Enquiry
	const char ack	=	0x06;	//		Acknowledgement
	const char bel	=	0x07;	// \a	Bell
	const char bs	=	0x08;	// \b	Backspace
	const char ht	=	0x09;	// \t	Horizontal tab
	const char lf	=	0x0A;	// \n	Line feed
	const char vt	=	0x0B;	// \v	Vertical tab
	const char ff	=	0x0C;	// \f	Form feed
	const char cr	=	0x0D;	// \r	Carriage return
	const char so	=	0x0E;	//		Shift out
	const char si	=	0x0F;	//		Shift in
	const char dle	=	0x10;	//		Data Link Escape
	const char dc1	=	0x11;	//		Device Control 1
	const char dc2	=	0x12;	//		Device Control 2 or XON
	const char dc3	=	0x13;	//		Device Control 3
	const char dc4	=	0x14;	//		Device Control 4 or XOFF
	const char nak	=	0x15;	//		Negative Acknowledgement
	const char syn	=	0x16;	//		Syncronous idle
	const char etb	=	0x17;	//		End of Transmission Block
	const char can	=	0x18;	//		Cancel
	const char em	=	0x19;	//		End of Medium
	const char sub	=	0x1A;	//		Substitute
	const char esc	=	0x1B;	// \e	Escape
	const char fs	=	0x1C;	//		File Separator
	const char gs	=	0x1D;	//		Group Separator
	const char rs	=	0x1E;	//		Record Separator
	const char us	=	0x1F;	//		Unit Separator

	const char del	=	0x7F;	//		Delete

	const char xon	=	0x11;	//		Resume Transmission
	const char xoff	=	0x13; 	//		Pause Transmission
}

#endif // _MODEL_CHARNAMES_H_

