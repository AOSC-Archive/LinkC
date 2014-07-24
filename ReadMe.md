
# LinkC
    A simple IM client based on Qt and use POSIX socket.
---
    LinkC是一个力求轻量的网络聊天软件[你可以认为我是在造轮子]
    与其他IM相比：
      - LinkC 更注重实用而非界面的漂亮，我们会同时提供GUI版本和NonUI版本的客户端
      - LinkC 相比其他IM，更倾向于去中心化，也就是我们希望在没有服务端的情况下，客户端还能继续运行一会儿
      - LinkC 纯属于开源软件，你可以拷贝服务端做任何修改然后重新发布[但请你遵守法律和道德约束]
      - LinkC 将注重安全和隐私[当然相关模块还未完全实现]，源码开放。
    开发现状：
      - LinkC 现在开发进度不算很快，又没有多少人参与，准确的说是一个冷门的IM，但是本人还是希望继续开发下去
      - LinkC 现在只做到了P2P链接部分，也就是可以使用LinkC通信的阶段。
      - LinkC 自行设计的LC系列通讯协议[基于TCP或UDP]稳定性还不强，有很多隐藏的BUG还未发现
    未来目标：
      - 完成一个完善的P2P网络，做到服务端负载低，以客户端为主的网络系统
      - 提高整体网络安全性，保护个人资料等不被窃取
      - 坚持轻量化。
    总体来说 LinkC 现在的可用性不高，希望有更多的人参与这个项目[当然，在开发过程中也会学到很多东西]


## Start Date:
    March 6, 2013

## Developer (so far):
    Junfeng Zhang  --- <SternWZhang@outlook.com>

## Build Dependencies:
    libsqlite3-dev qt-4(or qt-5)
## Runtime Dependencies:
    qt-4(or qt-5)

## Directories
    NewServer --> LinkC 的新服务端
    NewClient --> LinkC 的新客户端
    server    --> LinkC 的旧服务端[和旧客户端可以协作]
    user      --> LinkC 的旧客户端[和旧服务端可以协作]
    testing   --> 一些只在测试阶段的临时代码[可能有参考意义]
