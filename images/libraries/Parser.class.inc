<?php
    /*
     * Main Parser class which decides the parser to use based on the responseType provided by the user
     */
    require_once "../autoLoadClasses.php";

    class Parser {
        public $responseType;
        public $main;

        public function __construct($responseType) {
            $this->responseType = $responseType;
            if($responseType == 'web') {
                $this->main = new WebParser();
            } else if($responseType == 'image') {
                $this->main = new ImageParser();
            } else if($responseType == 'videos') {
                $this->main = new VideosParser();
            } else if($responseType == 'news') {
                $this->main = new NewsParser();
            } else if($responseType == 'apps') {
                $this->main = new AppsParser();
            } else if($responseType == 'books') {
                $this->main = new BooksParser();
            } else {
                $this->main = null;
            }
        }
    }