#!python
# -*- coding: utf-8 -*-
#
import os
import sys
if sys.version_info < (3, 0):
    import ConfigParser
else:
    import configparser
import tools.osutils
import xml.dom.minidom
import traceback

class BatchCopyPhase(object):

    def __init__(self, source_dir, target_dir, config_file, app_config):
        self._source_dir = source_dir
        self._target_dir = target_dir
        self._config_file = config_file

        self._version = ''
        self._force_new = False
        if app_config != '':
            try:
                doc = xml.dom.minidom.parse(app_config)
                rootNode = doc.documentElement
                versionNode = rootNode.getElementsByTagName("productversion")
                self._version = versionNode[0].firstChild.data
                self._force_new = True
            except:
                traceback.print_exc()
                print("parse xml failed!")

    def batch_copy(self):
        if sys.version_info < (3, 0):
            config = ConfigParser.SafeConfigParser()
        else:
            config = configparser.ConfigParser()
        # depends中，必须允许大写的key
        config.optionxform = str
        config.read(self._config_file)

        for package in config.sections():
            target_dir = os.path.join(self._target_dir, config.get(package, 'todir'))
            if self._version != '':
                target_dir = target_dir.replace('{version}', self._version)

            if self._force_new or (not os.path.isdir(target_dir)):
                tools.osutils.delete(target_dir)
                tools.osutils.mkdir(target_dir)
                
            tools.osutils.copy(target_dir, [
                {
                    'dir': os.path.join(self._source_dir, config.get(package, 'dir')),
                    'include': config.get(package, 'include').split(','),
                    'exclude': config.get(package, 'exclude').split(','),
                },
            ])


if __name__ == '__main__':
    if len(sys.argv) < 4:
        if sys.version_info < (3, 0):
            print >> sys.stderr,'*** Error: Not enougth parameter ***'
        else:
            print('*** Error: Not enougth parameter ***')
        sys.exit(1)

    app_config = ''
    if len(sys.argv) > 4:
        app_config = sys.argv[4]

    phase = BatchCopyPhase(sys.argv[1], sys.argv[2], sys.argv[3], app_config)
    phase.batch_copy()
