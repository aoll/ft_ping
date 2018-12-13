FROM ubuntu

RUN apt-get update \
	&& apt-get install git -y \
	&& apt-get install emacs -y \
	&& apt-get install -y make \
	&& apt-get install -y clang \
	&& apt-get install software-properties-common -y \
	&& apt-get update \
	&& apt-get install iputils-ping -y
	#&& apt-get install curl -y
	#&& apt-get install -y gnupg gnupg2  gnupg1 \
	#&& apt-get install -y apt-transport-https \
	#&& curl -sL https://packagecloud.io/AtomEditor/atom/gpgkey |  apt-key add - \
	#&& sh -c 'echo "deb [arch=amd64] https://packagecloud.io/AtomEditor/atom/any/ any main" > /etc/apt/sources.list.d/atom.list' \
