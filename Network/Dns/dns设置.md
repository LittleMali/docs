# dns设置

当前使用的dns服务器是什么？  
`/etc/resolv.conf`  
文件中多行nameserver对应主dns服务器，备dns服务器。  

`/etc/resolv.conf`中的dns服务器是系统自动设置的，例如配置了DHCP获取dns服务器，那么NetworkManager会自动刷新conf文件更新dns服务器。

## 设置为自动获取dns
在Linux系统中，你可以通过编辑网络接口配置文件或使用nmcli（NetworkManager命令行工具）来设置自动获取DNS服务器。具体的步骤可能会因你的Linux发行版和网络接口的类型而不同。

以下是在基于Debian的系统（如Ubuntu）中通过编辑网络接口配置文件来设置自动获取DNS服务器的步骤：

打开终端。

使用sudo nano /etc/network/interfaces命令打开网络接口配置文件。

找到你想要修改的网络接口的配置，如eth0。

确保该接口的配置中包含iface eth0 inet dhcp。这将设置该接口为使用DHCP，从而自动获取IP地址和DNS服务器。

按Ctrl+X，然后按Y，最后按Enter来保存并退出。

使用sudo systemctl restart networking命令重启网络服务，使新的设置生效。

以下是在使用NetworkManager的系统中通过nmcli来设置自动获取DNS服务器的步骤：

打开终端。

使用nmcli con show命令查看所有的网络连接。

找到你想要修改的网络连接的名称。

使用sudo nmcli con mod "连接名称" ipv4.method auto命令设置该连接为自动获取IP地址和DNS服务器。

使用sudo nmcli con up "连接名称"命令重新连接该网络，使新的设置生效。

请注意，你需要将"连接名称"替换为你实际的网络连接名称。

## 设置为手动dns
在Linux系统中，你可以通过编辑/etc/resolv.conf文件或使用nmcli（NetworkManager命令行工具）来手动设置DNS服务器。具体的步骤可能会因你的Linux发行版和网络接口的类型而不同。

以下是通过编辑/etc/resolv.conf文件来手动设置DNS服务器的步骤：

打开终端。

使用sudo nano /etc/resolv.conf命令打开DNS配置文件。

在文件中添加或修改nameserver行，指定你想要使用的DNS服务器的IP地址。例如，如果你想使用Google的公共DNS服务器，你可以添加以下行：

复制
nameserver 8.8.8.8
nameserver 8.8.4.4
按Ctrl+X，然后按Y，最后按Enter来保存并退出。

请注意，这种方法的一个缺点是，如果你的系统使用DHCP或网络管理器，那么这个文件可能会被自动覆盖。在这种情况下，你可能需要在你的网络接口配置或网络管理器设置中手动指定DNS服务器。

以下是在使用NetworkManager的系统中通过nmcli来手动设置DNS服务器的步骤：

打开终端。

使用nmcli con show命令查看所有的网络连接。

找到你想要修改的网络连接的名称。

使用以下命令设置该连接的DNS服务器：

复制
sudo nmcli con mod "连接名称" ipv4.dns "8.8.8.8 8.8.4.4"
sudo nmcli con mod "连接名称" ipv4.ignore-auto-dns yes
这将设置该连接的DNS服务器为Google的公共DNS服务器，并忽略通过DHCP获取的DNS服务器。

使用sudo nmcli con up "连接名称"命令重新连接该网络，使新的设置生效。

请注意，你需要将"连接名称"替换为你实际的网络连接名称，将"8.8.8.8 8.8.4.4"替换为你想要使用的DNS服务器的IP地址。