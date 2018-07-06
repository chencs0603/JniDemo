package personal.chencs.practice.jni;

import lombok.extern.slf4j.Slf4j;

import java.util.Properties;

/**
 * 本地接口
 *
 * @author: chencs
 * @date: 2018/6/25
 */
@Slf4j
public class KeyNativeUtils {

   static{
      // 读取配置文件的文件名
      Properties properties = new Properties();
      try {
         properties.load(KeyNativeUtils.class.getClassLoader().getResourceAsStream("native_dependency.properties"));
      } catch (Exception e) {
         log.error("fail to load the configuration file", e);
         throw new RuntimeException("fail to load the configuration file");
      }

      String simpleName = properties.getProperty("simpleName");
      if ("".equals(simpleName.trim()) || null == simpleName) {
         log.error("the content of configuration file is error");
         throw new RuntimeException("the content of configuration file is error");
      }
      // 加载本地依赖库
      LoadNativeLibUtils.load(simpleName);
   }

   /**
    * 获取两个密钥元素（演示方便：随机生成实现）
    *
    * @param keyElement1 密钥元素1，作为输出，调用之前需要分配足够的长度
    * @param keyElement1Len 密钥元素1的实际长度，作为输出
    * @param keyElement2 密钥元素2，作为输出，调用之前需要分配足够的长度
    * @param keyElement2Len 密钥元素1的实际长度，作为输出
    * @return 错误码，0表示成功
    */
   public static native int getKeyElements(byte[] keyElement1, int[] keyElement1Len, byte[] keyElement2, int[] keyElement2Len);

   /**
    * 根据密钥元素获取密钥（演示方便：两个密钥元素拼接）
    *
    * @param keyElement1 密钥元素1
    * @param keyElement2 密钥元素2
    * @return 错误码，0表示成功
    */
   public static native byte[] getKey(byte[] keyElement1, byte[] keyElement2);

   /**
    * 数组切分
    *
    * @param bytes 数组
    * @param blockLen 分组长度
    * @return 二维数组
    */
   public static native byte[][] arraySplit(byte[] bytes, int blockLen);

}
