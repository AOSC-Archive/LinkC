# LinkC Protocol   
## 简述    
LinkC 协议基于JSON（ 没有更多的说明了）    
## 启动连接(Start up connection)    
### 请求
```
"id"        : "id",    
"cmd"       : "connect",
"params"    : {
  "protocol"  : "gurgle",
  "version"   : "unusable",
  "encrypt"   : "disabled"
}
```
现在协议只支持 "gurgle"   
现在版本只存在 "unusable"   
现在加密只支持 "disabled"   
### 回应 (未修改进程序)
```
"id"        : "message's id",
"cmd"       : "kill",    
"params"    : {
  "error"     : "short description of error"/null,
  "reason"    : "human-readable description of reason"/null,
}
"reply"     : {
  "status"    : "connection established/failed",
  "error"     : "short description of error",
  "reason"    : "human-readable description of reason",
  "help"      : "help text or null"
},
```
如果链接正常建立，则只存在"reply"字段中有 "status" : "connection established"
如果链接建立失败，则"reply":{"status":"connection failed","error":"","reason":""},"cmd":"kill"
其中,"cmd" : "kill"和"reply"字段是分立的，即"kill"只告诉你要断开连接的事实，而reply仅是connect命令的返回
也就是说"reply"是由于"connect"命令执行而带来的回执
如果要寻求"kill"所带有的参数，请参照"params"字段
当然在这里，kill是不可能带参数的所以祝君放心，毕竟发两个同样的错误回执真是太差劲了

## 检查延迟(ping)
### 请求
```
"id"        : "id",    
"cmd"       : "ping"    
```
### 回应
```
"id"        : "message's id",    
"reply"     : "pong"
```

## 获取认证方式(Get authentication method)
### 请求
```
"id"        : "id",    
"cmd"       : "query",   
"params"    : {
    "target"        : "auth_method",
    "opinion"       : "list_enabled(default)",
    "replay_style"  : "text(default)"
  }
```
在params中的为可选参数，留空就意味着使用缺省配置    
- target  
  - auth_method    本次请求的为认证方式，故target为auth_method
- opinion     
  - list_enabled   返回服务端目前全部支持的认证方式   
- replay_style      
  - text		       返回文本定义     

### 回应
```
"id"        : "message's id",    
"reply"     : {
  "auth_method"   : ['plain_password']，
  "error"         : null
}
``` 
回复值为一个list

## 获取认证状态(Get authentication status)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "auth_status",
  "error"     : null
}
```
### 回应
```
"id"        : "message's id",    
"reply"     : {
  "auth_status"   : "Unauthenticated/Authenticated",
  "error"         ： null
}
``` 

## 强制关闭连接(Force client disconnect)
### 命令:
```
"id"        : "request id or 0",
"cmd"       : "kill",
"params"    : {
  "error"     : "error"/null,
  "reason"    : "reason/null"
}
```
这是只能由服务端发送
服务端发送完报文后就会关闭连接

## 获取协议版本(Get protocol's version)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "version"
}
```
### 回应
```
"id"        : "message's id",    
"reply"     : {
  "version"   : "server's version",
  "error"     : null
}
``` 

## 明文认证(Plain password authentication)
### 请求:
```
"id"        : id,
"cmd"       : "auth", 
"from"      : gurgle_id, 
"params": {
  "method"    : "plain_password",
  "password"  : "password"
  }
}
```
### 回应:
```
"id"  : id, 
"to"  : full_gurgle_id, 
"reply" : {
  "error"   : null,
  "reason"  : null
}
```
to字段严格指明你现在登陆的ID，严格遵守ID的格式。
如果在之前没有指明terminal字段，这里也会为你分配一个
如果登陆失败，to字段将省略，或者设置为null(目前是省略掉了的，并且reason字段也是省略了的)

## 更新自己状态更新(Publishing self presence update)
### 推送:
```
"id"          : id,
"cmd"         : "push",
"params"      : {
  "target"      : "presence",
  "last_name"   : "new last_name"/None,
  "first_name"  : "new first_name"/None,
  "status"      : "Avaliable/Away/Dnd/Invisible"/None,
  "mood"        : "Your new mood"/None
}
```
None则意味着你想将这个字段置空    
如果不想更改某个字段，可以选择提交原来的状态   
或者不提交这个字段也是可以的
比如：  
```
  "id"        : "id",
  "cmd"       : "push",        
  "params"     : {    
    "target"      : "presence",        
    "last_name"   : "last_name"/None,     
  }    
```
注释：仅修改你的名

### 回应:
```
"id"        : "message's id",    
"reply"     : {
  "error"     : "PermissionDenied"/OtherErrors/null,
  "reason"    : "Unauthenticated"/null
}
```

## 请求自己状态(Get self presence)
### 请求:
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "presence"
}
```
### 回应
```
"id"        : "message's id",    
"reply"     : {
  "last_name"   : "last_name"/None,
  "first_name"  : "first_name"/None,
  "status"      : "Avaliable/Away/Dnd/Invisible",
  "mood"        : "Your mood"/None
  "error"       : null,
  "reason"      : null
}
``` 
## 请求好友列表(Query roster)
### 请求:
```
"id"        : id,
"cmd"       : "query",
"params"    : {
  "target"    : "roster",
  "limit"     : 100
}
```
target是必选参数，limit是可选参数
limit字段指定最大返回好友item的个数，如果不指定这个参数，默认limit为0，也就是无上限

## 推送好友列表(Push roster)
### 推送
```
"id"        : 0,
"cmd"       : "push",
"params"    : {
  "target"    : "roster",
  "count"     : 0,1,2,3.......
  "value"     : [
    ["nickname","last_name","first_name","status","mood"],
    ["nickname","last_name","first_name","status","mood"],
    ......
  ]
}
```

## 输出错误(Print errors)
若客户端发来错误格式的报文，予以错误回复
### 回复
```
"id"        : "message's id",
"reply"     : {
  "error"     : "SyntaxError",
  "reason"    : details
  "help"      : "A string to tell you the right syntax"
}
```

## 加入组/添加好友(Friend/group request)
### 请求:
```
"id"        : request_id,
"cmd"       : "request"
"params"    : {
  "cmd"       : "request",
  "params"    : {
    "addition"  : "....."
  }
}
```
Id为0表示不需要即时回复

### 转发:
```
"id"        : new id,
"from"      : "original address",
"cmd"       : "request",
"params"    : {
  "addition"  : "....."
}
```

### 回复:
```
"id"        : 0,
"cmd"       : "forward",
"to"        : "original address",
"params"    : {
  "reply"     : {
    "status"  : "accepted/refused",
    "reason"  : "reason",
    "error"   : null
  }
}
```

### 转发:
```
"id"        : 0,
"from"      : "A's address"
"reply"     : {
  "status"    : "accepted/refused",
  "reason"    : "reason",
  "error"     : null
}
```

## 订阅相关
### 请求订阅某人/某群组:
```
"id"        : id_A,
"cmd"       : "subscribe",
"params"    : {
  "to"        : "gurgle id",
  "addition"  : "something to say"
}
```
### 某人/某群组收到的为:
```
"id"        : id_B,
"cmd"       : "push",
"params" : {
  "target"    : "subscribed_request",
  "from"      : "gurgle id",
  "addition"  : "something to say"
}
```
### 某人/某群组回复为:
```
"id"        : id_B,
"cmd"       : "subscribe_reply",
"params"      : {
  "status"    : "accepted/refused/ignored",
  "to"        : "gurgle id",
  "addition"  : "something to say" [拒绝和允许都可以发送附加信息给别人]
}
```
### 请求者收到的回复为
```
"id"        : id_A,
"cmd"       : "reply",
"params"    : {
  "status"    : "accepted/refused",
  "addition"  : "something to say"/null
}
```