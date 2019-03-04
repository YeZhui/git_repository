目录介绍：
en_decrypt		加密算法库，目前实现md5、Base64加密、RSA算法
log				日志类库，目前支持输出到屏幕、输出到文件、输出到系统syslog
mysql			linux下读写mysql
readini 		配置文件库，目前只支持读配置文件
network			封装epoll，它依赖于log和readini，需配合使用
ftp				使用tcp实现ftp协议，目前只实现了客户端功能
test			单元测试代码
bash			存放统一makefile和脚本

编译步骤：
pwd   //假设当前在toollib目录下
cd bash
sh cp_makefile.sh
cd ..
make
