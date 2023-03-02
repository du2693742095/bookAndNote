# vimplus安装

## 检查vim版本
首先查看一下vim的版本 检查一下是否是8.0及以上的版本。现在ubunbtu默认的都是8.0以上的版本

```shell
$ vim --version
```
然后查看一下当前的im是否支持python3 若出现 +python3这一项表示支持,-python3表示不支持
```shell
$ vim --version l grep python
```


## 安装vimpluS插件
### 获取安装包
方式一: 直接从git clone下牙
```shell
$ git clone https://github.com/chxuan/vimplus.git ~/vimplus-master
```


方式二: 直接用离线
如果上面git clone很慢，半天下载不下来，可以直接用已经下载好的的vimplus-master.zip
```shell
$ unzipvimplus-master.zip
```
安装
```shell
$ cd ~/vimplus-master
$·/instal1.s
```
等待过程中, 会出现让你用python2还是python3编译自动全插件YouCompleteMe(ycm), 这里选择3即可。


### 插件安装
vimplus安装完后会自动进入插件的安装，我们会看到一个各种插件的下载界面，如果此时插件正常下载，可以等待下载完成; 如果插件下载半天没动静, 或者很多插件都下载不成功，可以按下面的方法进行完成后续的安装。
1. vimplus将所有的vim插件都放在-/.vim/pluged目录下,可以进入该目录进行查看。
```shell
$ cd ~/.vim/plugged
$ ls
```
2. 拷贝plugged.zip文件到你的linux上,然后执行命令
```shell
$ unzip plugged.zip
```

这样会得到一个plugged目录, 然后我们将原本的plugged删除，将我们解压得到的plugged移动过去
```shell
$ rm -rf ~/.vim/plugged
$ mv plugged ~/.vim
```
3、最后再写入我们的配置文件, 这里可以直接用vimrcbf, 将此时的.vimrc删除或者备份一份也可.
```shell
$ rm -f ~/.vir
$ mv vimrcbf .vimrc
```
现在可以试试vim是否已经具备功能了。 进入vim之后在normal模式下, 按下\<leader\> +h，即可打开帮助文档，leaders默认为逗号" ，可以在.vimrc进行修改，很多功能的快捷键均可在vimrc进行修改。