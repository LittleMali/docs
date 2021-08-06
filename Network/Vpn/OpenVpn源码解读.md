

openvpn_main
    init_static
    gc_init
    init_options
    parse_argv
    init_options_dev
    tunnel_point_to_point
        c->mode = CM_P2P;
        init_instance_handle_signals(c, c->es, CC_HARD_USR1_TO_HUP);
            init_instance
                do_event_set_init
                    c->c2.event_set = event_set_init(&c->c2.event_set_max, flags);
                        event_set_init_simple
                            we_init
                                wes->func.free = we_free;
                                wes->func.reset = we_reset;
                                wes->func.del = we_del;
                                wes->func.ctl = we_ctl;
                                wes->func.wait = we_wait;
                link_socket_init_phase1 // bind the TCP/UDP socket
                do_open_tun
                    do_init_tun
                    open_tun
        while (1)
            io_wait(c, p2p_iow_flags(c));
            ~~~p2p_iow_flags
                #define IOW_READ  (IOW_READ_TUN|IOW_READ_LINK) // 注意，这里初始化flags时就包括了读数据
                flags = (IOW_SHAPER|IOW_CHECK_RESIDUAL|IOW_FRAG|IOW_READ|IOW_WAIT_SIGNAL);
                if (c->c2.to_link.len > 0)
                {
                    flags |= IOW_TO_LINK;
                }
                if (c->c2.to_tun.len > 0)
                {
                    flags |= IOW_TO_TUN;
                }

            ~~~io_wait
                if fastio
                    // win下没有开启，就不看了
                else
                    io_wait_dowork(c, flags);
                        if (flags & IOW_WAIT_SIGNAL)
                            wait_signal(c->c2.event_set, (void *)&err_shift);
                        if (flags & IOW_TO_LINK)
                            ...
                        if (flags & IOW_TO_TUN)
                            ...
                        socket_set
                        tun_set

                        if (!c->sig->signal_received)
                            event_wait(c->c2.event_set, &c->c2.timeval, esr, SIZE(esr));
            process_io
                if (status & SOCKET_WRITE)
                    process_outgoing_link(c);
                if (status & TUN_WRITE)
                    process_outgoing_tun(c);
                if (status & SOCKET_READ)
                    read_incoming_link(c);
                if (status & TUN_READ)
                    read_incoming_tun(c);

// 浏览器的数据包被路由到tun设备，触发tun读取
status & TUN_READ ==> read_incoming_tun
    tuntap->windows_driver == WINDOWS_DRIVER_TAP_WINDOWS6
        read_tun_buffered(c->c1.tuntap, &c->c2.buf);
            tun_finalize(tt->hand, &tt->reads, buf);
                // 到这里，我们要去找这个switch-case是在哪里赋值的。
                switch (io->iostate)
                    case IOSTATE_QUEUED:
                        GetOverlappedResult
                        if (success)
                            io->iostate = IOSTATE_INITIAL;
                            ret = io->size;
                        else
                            if (lastErr != ERROR_IO_INCOMPLETE)
                                io->iostate = IOSTATE_INITIAL;

                    case IOSTATE_IMMEDIATE_RETURN:
                        io->iostate = IOSTATE_INITIAL;
                        if (io->status)
                            ret = -1
                        else
                            ret = io->size;

                    case IOSTATE_INITIAL:
                        ret = -1;

// 既然是驱动抛上来的数据，那么应该跟驱动有点关系
multi_tcp_wait -- tunnel_server -> tunnel_server_tcp 一路调过来的，不用跟

io_wait
io_wait_dowork(c, flags);

    tun_set
        tun_read_queue(tt, 0);

有什么区别
enum windows_driver_type {
    WINDOWS_DRIVER_UNSPECIFIED,
    WINDOWS_DRIVER_TAP_WINDOWS6,
    WINDOWS_DRIVER_WINTUN
};

event_set_functions

