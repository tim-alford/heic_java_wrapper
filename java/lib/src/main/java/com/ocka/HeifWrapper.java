// vim: ts=2
package com.ocka;
public class HeifWrapper {
	static {
		try{
				// make sure that library is in /usr/lib or /lib
				// not /usr/local/lib ...
				// default java path doesn't see this directory
				//System.out.printf("Path = %s\n", System.getProperty("java.library.path"));
				System.loadLibrary("ockaheif");
		}catch(Exception x){
			throw new RuntimeException("Caught exception while loading native library", x);
		}
	}
	public native String getVersion();

	public native ImageData getImageData(String fileName);
}
