// vim: ts=2
package com.ocka.model;
public class ImageData {
	public int width; // width of image (pixels)
	public int height; // height of image (pixels)
	public int stride; // stride of data
	public int length; // length of array
	public int[] bytes; // binary data
	public boolean error;
	public String message;
}
