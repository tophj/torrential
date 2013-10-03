torrent::initialize();
    torrent::main_thread()->slot_do_work() = tr1::bind(&client_perform);
    torrent::main_thread()->slot_next_timeout() = tr1::bind(&client_next_timeout, control);

    worker_thread = new ThreadWorker();
    worker_thread->init_thread();




worker_thread->start_thread();

    torrent::thread_base::event_loop(torrent::main_thread());



 delete worker_thread;
