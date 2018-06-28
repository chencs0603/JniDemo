package personal.chencs.practice.jni;

import lombok.extern.slf4j.Slf4j;

import java.io.*;

/**
 * 工具类：加载本地依赖库
 *
 * @author chencs
 * @since 2018/6/22
 */
@Slf4j
public class LoadNativeLibUtils {

    /**
     * 加载本地依赖库
     *
     * @param simpleName 文件名（不包括扩展名）
     * @throws IOException
     */
    public synchronized static void load(String simpleName) {
        InputStream inputStream = null;
        File destFile = null;
        BufferedInputStream reader = null;
        FileOutputStream writer = null;

        try {
            // 根据操作系统类型，判断依赖库的扩展名
            // windows:dll, linux:so
            String systemName = System.getProperty("os.name");
            log.debug("systemName:{}", systemName);
            String extensionName = (systemName.toLowerCase().indexOf("wind") != -1) ? ".dll" : ".so";
            String fullName = simpleName + extensionName;

            // 获取临时目录
            String tempIODir = System.getProperty("java.io.tmpdir");
            log.debug("tempIODir:{}", tempIODir);
            destFile = new File(tempIODir + File.separator + fullName);
            // 如果存在同名文件，应先删除，保证每次启动加载都是jar包中最新的本地依赖库
            destFile.deleteOnExit();
            // 读取jar包中的动态库文件
            inputStream = LoadNativeLibUtils.class.getClassLoader().getResourceAsStream(fullName);
            if (null == inputStream) {
                log.error("fail to load the native library in the jar package");
                throw new RuntimeException("fail to load the native library in the jar package");
            }
            // 将jar包中的依赖库写入到临时目录
            reader = new BufferedInputStream(inputStream);
            writer = new FileOutputStream(destFile);
            byte[] buffer = new byte[1024];
            while (reader.read(buffer) > 0) {
                writer.write(buffer);
            }
            writer.flush();

        } catch (Exception e) {
            log.error("fail to copy the native library from the jar package to the temp directory");
            throw new RuntimeException("fail to copy the native library from the jar package to the temp directory");
        } finally {
            try {
                if (null != writer) {
                    writer.close();
                }
            } catch (Exception e) {
                log.error("fail to close the writer");
                throw new RuntimeException("fail to close the writer");
            }
            try {
                if (null != reader) {
                    reader.close();
                }
            } catch (Exception e) {
                log.error("fail to close the reader");
                throw new RuntimeException("fail to close the reader");
            }
            try {
                if (null != inputStream) {
                    inputStream.close();
                }
            } catch (Exception e) {
                log.error("fail to close the inputStream");
                throw new RuntimeException("fail to close the inputStream");
            }
        }
        // 加载依赖库
        System.load(destFile.toString());
        log.info("loaded the native library");
    }

}
