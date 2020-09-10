# -*- coding: utf-8 -*-

import logging
import tarfile

def make_tar_gz(tar_file_path, contents):
    u'''
    tar_file_path tar.gz的路径
    contents是一个list of (需要添加的路径，对应的arcname)
    '''

    logger = logging.getLogger(__name__)

    if len(contents) == 0:
        logger.debug('content is empty, tar.gz won\'t be made')

    tar_gz_file = tarfile.open(tar_file_path, 'w:gz')

    for item in contents:
        tar_gz_file.add(item[0], arcname=item[1])

    tar_gz_file.close()

