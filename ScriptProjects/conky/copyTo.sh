#Copy to ~/.conky
rsync -avz --exclude-from='exclude-file.txt' ./* ~/.conky

