// vim: ts=2
package com.ocka.heif;
import com.ocka.model.*;
public class HeifWrapper {
	static {
		try{
				// make sure that library is in /usr/lib or /lib
				// not /usr/local/lib ...
				// default java path doesn't see this directory
				System.loadLibrary("ockaheif");
		}catch(Exception x){
			throw new RuntimeException("Caught exception while loading native library", x);
		}
	}
	public native String getVersion();

	public native ImageData getImageData(String fileName);
}
