basic_client:
	$(MAKE) -C basic_client

basic_server:
	$(MAKE) -C basic_client

epoll_server:
	$(MAKE) -C epoll_server
