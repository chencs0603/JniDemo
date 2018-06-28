package personal.chencs.practice.jni;

import org.apache.commons.codec.binary.Hex;
import org.junit.Assert;
import org.junit.Test;

/**
 * @author: chencs
 * @date: 2018/6/25
 */
public class KeyNativeUtilsTest {

    @Test
    public void getKeyElements() {
        byte[] keyElement1Temp = new byte[0x10];
        int[] keyElement1Len = {0x10};
        byte[] keyElement2Temp = new byte[0x10];
        int[] keyElement2Len = {0x10};

        int result = KeyNativeUtils.getKeyElements(keyElement1Temp, keyElement1Len, keyElement2Temp, keyElement2Len);
        Assert.assertTrue(0 == result);

        byte[] keyElement1 = new byte[keyElement1Len[0x00]];
        System.arraycopy(keyElement1Temp, 0x00, keyElement1, 0x00, keyElement1.length);
        byte[] keyElement2 = new byte[keyElement2Len[0x00]];
        System.arraycopy(keyElement2Temp, 0x00, keyElement2, 0x00, keyElement2.length);

        System.out.println("KeyElement1:" + Hex.encodeHexString(keyElement1));
        System.out.println("KeyElement2:" + Hex.encodeHexString(keyElement2));

        byte[] key = KeyNativeUtils.getKey(keyElement1, keyElement2);
        Assert.assertTrue(null != key);
        System.out.println("key:" + Hex.encodeHexString(key));
    }

}
