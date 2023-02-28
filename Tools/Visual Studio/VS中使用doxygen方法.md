版本是从 `v16.6` 开始的 `Visual Studio 2019` , 步骤如下:

先切换到 Doxygen，然后转到Tools > Options > Text Editor > C/C++ > General ( 工具 > 文本编辑器 > C/C++ > 代码样式 > 生成的文档注释样式 )，然后选择你喜欢的文档样式：

![doxgen位置](https://i.stack.imgur.com/t6S2i.png)



通过指定以后, 可以直接通过 `///` 或者 `/**` 来使用

![doxgen使用](https://upload-images.jianshu.io/upload_images/28872667-ee72f5de9e88c833.gif?imageMogr2/auto-orient/strip)



这样的好处是: 当你引用一个函数时, 会提示你相应的参数及其含义, 效果入图所示:
![使用实例](https://upload-images.jianshu.io/upload_images/28872667-34f363fc22a49d8a.gif?imageMogr2/auto-orient/strip)



出现的过问题:

1. 使用 `///` 的时候弹出来 `<summery>` 的注释风格, doxgen风格无效: 这是没有先去 C/C++ 里面选择doxgen注释风格, 选择了以后就好了
2. 使用 `/**` 的时候无效, 只能用 `///` 或者 `ctrl + /` : 原因是有快捷键冲突了, 微软团队解决过这个问题, 需要升级插件版本, 解决的问题在vscode的官方issue里面提到了: https://github.com/microsoft/vscode-cpptools/issues/9799#event-7439131868

