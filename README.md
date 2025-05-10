# ImageSR Toolkit

开发至此，已经可用，但还有相当多不完善之处。采取命令行调用的形式，一条命令一次处理（批处理），也算是返璞归真。

## 使用方法

1. 在该文件夹下打开命令提示符 (cmd)。操作：在文件管理器地址栏内输入 "cmd"（不带引号），然后回车。
2. 调用主程序 ImageSR.exe。操作：输入 "ImageSR.exe --help"（不带引号），回车即可看到使用说明。
3. 按照说明补全你的命令（command），回车运行即可，程序会自动识别文件 / 文件夹。

[开发日志](https://www.bilibili.com/video/BV1jJVuzBEDD) 稍提到了一些使用方法，不妨看看。

**如果程序闪退，大概是你的配置有问题；如果程序抛出了异常说明，并且你看不懂，大概是代码有 BUG。**

这是一个合法的命令例子（为了方便解释换了行）：

```bat
ImageSR.exe
	-i "xxx\\in.jpg"       # '-i' 为输入路径的标示开关，后面跟着待处理的文件路径。注意转义字符 "\\"。
	--output "D:\\out.jpg" # '--output' 为输出路径的标示开关，后跟输入路径。
	                         # 此开关也可记作 '-o'，可以使用 '-h' 查看开关名称。
	-m realesrgan-anime    # 你要使用的模型，详解见下文。
	-s 4                   # 图片放大倍数，配合着模型使用，可能还需要 '--denoise' 和 '--syncgap' 开关。
	--extension jpg png    # 文件后缀名选择器，在此表示只处理 jpg 和 png 格式的图片。
	--recursive            # 递归处理子文件夹。
	--force                # 若输出路径存在，强制覆盖已有的文件。
```

实际上，上述的例子在 cmd 中长这样：

`ImageSR.exe -i "xxx\\in.jpg" -o "D:\\out.jpg" -m realesrgan-anime -s 4`（输入路径是一个文件时，上述后三个开关不是必须的）

## 模型介绍

命令除模型（--model）外都不难理解，故在此特别介绍模型部分。

模型配置一共四项：

- --model, -m
- --scale, -s
- --denoise, -d
- --syncgap, -g

其中前两项为必填，后两项根据模型不同可能必须要填。**你的配置中必须包含所用模型的所有属性。**

以下是工具箱支持的模型及其属性，加粗即为模型的默认属性，供参考：

- DF2K / DF2K-JPEG（realesr一代模型）
  
    - 放大倍数：**4**

- realesrgan / realesrnet / realesrgan-anime（realesr二代模型）
  
    - 放大倍数：**4**

- realesrgan-animevideo（realesr三代模型，使用内存，针对视频超分做了特异性优化）
  
    - 放大倍数：2 / 3 / **4**

- realcugan
  
    - 放大倍数：1 / **2** / 3 / 4
    - 降噪等级：**-1** / 0 / 1 / 2 / 3
    - 分块等级：0 / 1 / 2 / **3**

- waifu2x-anime / waifu2x-photo（waifu2x模型）
  
    - 放大倍数：1 / **2** / 4 / 8 / 16 / 32
    - 降噪等级：-1 / **0** / 1 / 2 / 3

## 注意事项

1. 关于模型之间的差异（最终效果请自行评判）

    - realesrnet & waifu2x-photo 模型适合超分真实图片。
    - realesrgan-anime 模型适合超分动漫图片。
    - realesrgan-animevideo 模型适合超分动漫视频（或者图片），其在细节上表现不如 realesrgan-anime，但处理速度更快。
    - realcugan 在线条纹理上表现较优，可以保留更多的细节。
   
3. 关于性能

    - AI 模型使用显卡放大图片，处理速度取决于显卡算力和图片分辨率。
    - 若没有独显或独显很弱，建议使用 realesrgan-animevideo 模型处理动漫图片，此模型处理速度最快。
    - 只有 realesrgan-animevideo 模型为Ram版本（内存版本），没有独显也可以超分大图片。
    - 对于非Ram版本的模型而言，显存和内存不足 $8\text{GiB}$ 可能会导致崩溃。
    - 受制于硬件性能，模型通常在处理大于 $30\text{MiB}$ 的图片时会崩溃。

4. 显卡测试1：RX588 ARCAEA-8K-HKT.png 16MiB 7680*4320 

    - realesrgan 模型，用时30min。
    - realesrgan-animevideo 模型，用时14min。

5. 关于显存占用问题

    - 如果使用独显，任务管理器中是看不到显卡占用的；但在NA软件中能看到已经跑满。
    - 集显占用可以直接在任务管理器中看到。

6. **对真实图片进行超分不能使用anime模型！** ~~会有意外惊喜~~
