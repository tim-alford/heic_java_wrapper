// vim: ts=2
package com.ocka;
import org.junit.Test;
import static org.hamcrest.MatcherAssert.assertThat; 
import static org.hamcrest.Matchers.*;
import java.awt.image.*;
import javax.imageio.ImageIO;
import java.io.File;
public class HeifWrapperTest {
    @Test 
		public void testLoadingLibrary() {
			HeifWrapper heif = new HeifWrapper();
			String version = heif.getVersion();
			System.out.printf("Version = %s\n", version);
    }
		@Test
		public void testReadingImageData() throws Exception {
			HeifWrapper heif = new HeifWrapper();
			String fileName = "/home/talford/IMG_8123.HEIC";
			ImageData data = heif.getImageData(fileName);
			assertThat(data, is(not(nullValue())));
			assertThat(data.error, is(equalTo(false)));
			assertThat(data.message, is(equalTo("Success")));
			assertThat(data.width, is(equalTo(2699)));
			assertThat(data.height, is(equalTo(3605)));
			assertThat(data.stride, is(greaterThan(0)));
			//assertThat(data.stride, is(equalTo(10796)));
			assertThat(data.bytes, is(not(nullValue())));
			assertThat(data.length, is(equalTo(data.stride*data.height)));
			BufferedImage image = new BufferedImage(data.width, data.height, BufferedImage.TYPE_INT_ARGB);
			// move image generation into image data ....
			for(int i = 0 ; i < data.height; i++){
				int offset = i * data.stride;
				for(int j = 0; j < data.width * 1 * 4; j+=4){ // width * bpp * number of channels
					int r = data.bytes[offset+j];	
					int g = data.bytes[offset+j+1];	
					int b = data.bytes[offset+j+2];	
					int a = data.bytes[offset+j+3];	
					int pixel = 0;
					pixel |= ((a & 0xFF) << 24);
					pixel |= ((r & 0xFF) << 16);
					pixel |= ((g & 0xFF) << 8);
					pixel |= (b & 0xFF);
					image.setRGB(j/4, i, pixel);
				}
			}
			File f = new File("/home/talford/test.png");
			ImageIO.write(image, "png", f);
			assertThat(f.exists(), is(true));
			System.out.printf("%s\n", f.getAbsolutePath());
		}
}
