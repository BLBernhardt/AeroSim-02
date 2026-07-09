@echo via loopback adaptor
py State.py 127.0.0.1 5001

@echo via multicast
py State.py 224.0.0.1 5001

rem @echo via unicast
rem py State.py 192.168.1 5001
