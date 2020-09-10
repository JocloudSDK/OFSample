# -*- coding: utf-8 -*-

u'''
    通过vboxmanage管理VirtualBox虚拟机

    构建脚本目前使用虚拟机进行smart prefetch的计算。
    对虚拟机有如下约定：
    1、虚拟机的硬盘设置为immutable，在完成初始化后，生成snapshot
    2、虚拟机的网络采用NAT
'''

import os
import subprocess


def clone_vm_linked(source_vm_name, snapshot_name):
    u'''
        通过link模式，复制一个vm，使其指向原始vm的指定snapshot
    '''

    # 同一个host上允许同时产生多个虚拟机，需要确保：
    #   1、虚拟机名称不重名
    #   2、NAT端口不冲突
    # 所以，利用端口号作为虚拟机名称。
    # 建立虚拟机时，virtualbox会确保虚拟机不重名，也就确保了端口不冲突。

    # 目前每台构建机上最多并行3个任务，理论上，3个名称就足够。
    # 多准备两个，容忍虚拟机没有删除干净的情况。
    name_port_map = {
        'prefetch-8080': '8080',
        'prefetch-8081': '8081',
        'prefetch-8082': '8082',
        'prefetch-8083': '8083',
        'prefetch-8084': '8084',
    }

    for vm_name, port in name_port_map.iteritems():
        returncode = subprocess.call(
            [
                os.path.join(
                    os.environ['VBOX_INSTALL_PATH'],
                    'VBoxManage.exe'),
                'clonevm',
                '%s' % source_vm_name,
                '--snapshot',
                '%s' % snapshot_name,
                '--options',
                'link',
                '--name',
                vm_name,
                '--register',
            ]
        )

        if returncode == 0:
            _setup_nat_forward_rule(vm_name, port)
            return vm_name, port
        else:
            # 可能是名称重复，也可能是其他错误，尽量尝试
            pass

    return None, None


def _setup_nat_forward_rule(vm_name, port):
    subprocess.check_call(
        [
            os.path.join(os.environ['VBOX_INSTALL_PATH'], 'VBoxManage.exe'),
            'modifyvm',
            vm_name,
            '--natpf1',
            'http,tcp,,%s,,8080' % port,
        ]
    )


def power_on_vm(cloned_vm_name):
    u'''启动虚拟机'''
    subprocess.check_call(
        [
            os.path.join(os.environ['VBOX_INSTALL_PATH'], 'VBoxManage.exe'),
            'startvm',
            cloned_vm_name,
        ]
    )


def power_off_vm(cloned_vm_name):
    u'''关闭虚拟机'''
    subprocess.check_call(
        [
            os.path.join(os.environ['VBOX_INSTALL_PATH'], 'VBoxManage.exe'),
            'controlvm',
            cloned_vm_name,
            'poweroff',
        ]
    )


def delete_vm(cloned_vm_name):
    u'''删除虚拟机，包括从virtualbox中反注册，并删除对应的文件夹'''
    subprocess.check_call(
        [
            os.path.join(os.environ['VBOX_INSTALL_PATH'], 'VBoxManage.exe'),
            'unregistervm',
            cloned_vm_name,
            '--delete',
        ]
    )


if __name__ == '__main__':
    # 示例代码，也可用于测试用途
    vm_name, port = clone_vm_linked('prefetch', 'fixed')
    if vm_name is not None:
        try:
            power_on_vm(vm_name)
            import time
            time.sleep(120)
            power_off_vm(vm_name)
        finally:
            delete_vm(vm_name)
