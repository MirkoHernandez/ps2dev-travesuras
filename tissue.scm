(tissue-configuration
 #:indexed-documents (append (map (lambda (filename)
				    (slot-set (read-gemtext-issue filename)
					      'web-uri
					      (string-append "/" (string-remove-suffix ".gmi" filename))))
				  (gemtext-files-in-directory "issues"))
			     (map (lambda (filename)
				    (slot-set (read-gemtext-document filename)
					      'web-uri
					      (string-append "/" (string-remove-suffix ".gmi" filename))))
				  (gemtext-files-in-directory "docs")))
 #:web-files (map (lambda (filename)
                    (file (replace-extension filename "html")
                          (gemtext-exporter filename)))
		  (append
		   (gemtext-files-in-directory "docs")
                   (gemtext-files-in-directory "issues"))))
