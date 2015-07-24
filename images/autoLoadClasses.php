<?php
    if(!function_exists('classAutoLoader')) {
        function classAutoLoader($class) {
            //Change the classFileRoot variable according to the folder organization
            $classFileRoot = $_SERVER['DOCUMENT_ROOT'].'/freegoogleapis/images/libraries/';
            $classFile= $classFileRoot . $class . '.class.inc';
            if(is_file($classFile)&&!class_exists($class)) {
                include $classFile;
            }
        }
    }
    spl_autoload_register('classAutoLoader');