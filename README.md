# libevilsocket

Client/Server socket wrapper on POSIX and Windows socket API.

# Implemented

- Basic socket operations for WinSock2
- TcpServer and TcpClient class wrappers 
- Send/Recv wrappers
- Simple error handling

# TODO

- Unix implementation for evilsocket functions
- Unix test implementation for client and server
- TCP KeepAlive option implementation
- TCP accept no-wait option
- Send/Recv per-byte implementation
- Server with multiple clients implementation
- Store client address in TcpClient object coming from accept
 - Where get address on es_connect?

# Credits

Author: TheEvilRoot

# License

```
MIT License

Copyright (c) 2021 TheEvilRoot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINgiGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
 
