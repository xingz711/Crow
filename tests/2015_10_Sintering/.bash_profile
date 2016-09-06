###########
###  START Custom Profile
###  Author: jason.miller@inl.gov
###  Date:   7-9-2014
# Set proxies for use on the terminal
if [ `ifconfig | grep -c "141.221.120\|141.221.124\|141.221.121"` -ge 1 ]; then
  echo 'Using FN Proxies'
  proxy="http://fnwebbalance.fn.inl.gov:8080"
elif [ `ifconfig | grep -c "134.20.\|141.221."` -ge 1 ]; then
  echo 'Using INL Proxies'
  proxy="http://webbalance.inl.gov:8080"
else
  # No Proxies
  echo 'No Proxies set'
  proxy=""
fi

if [ "x$proxy" != "x" ]; then
  proxy_array=(http_proxy https_proxy ftp_proxy ftps_proxy)
  for proxy_item in ${proxy_array[*]}; do
    export $proxy_item=$proxy
  done
fi
# End Set proxies
###

###
# Set Pretty stuff
PS1="\[\033[1;34m\][\u]\[\033[1;32m\][\w]\[\033[0m\]> "
export CLICOLOR=1
export LSCOLORS=exfxcxdxbxegedabagacad
#
###

###
# Set useful aliases
alias ll="ls -latrh"
alias l="ls -latrh"
# End aliases
###
###  END Custom Profile
###  you can safely append any custom content below this line
