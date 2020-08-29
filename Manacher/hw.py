#-*- coding:utf-8 -*-
'''
author: silentroot95
Created on Aug 29,2020
'''
def preProcess(s):
    '''
    马拉车算法字符串预处理
    '''
    #空字符串处理
    if len(s)==0:
        return '^$'
    t = '#'.join(list(s))
    t = '^#'+t+'#$'
    return t
def Manacher(s):
    '''
    马拉车算法
    '''
    t = preProcess(s)
    length = len(t)
    #数组p
    p = [0]*length
    C=R=0
    for i in range(1,length-1):
        i_mirror = 2*C-i
        #保证i在当前的字串中
        if(R>i):
            #保证不超过右边界
            p[i] = min(R-i,p[i_mirror])
        else:
            #i==R时
            p[i]=0
        #中心扩展
        while(t[i+p[i]+1] == t[i-p[i]-1]):
            p[i] += 1
        #更新C与R
        if(i+p[i]>R):
            C=i
            R=i+p[i]
    #最大回文串长度
    maxlen = max(p)
    #起始索引
    start = (p.index(maxlen)-maxlen) // 2
    return s[start:start+maxlen]
if __name__ =='__main__':
    s = 'cbcbcbde'
    sh = Manacher(s)
    print(sh)

