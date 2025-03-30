# Да се направят директории с имената на уникалните групи. За улеснение, имената
# от две думи да се напишат слято:

# Beatles, PinkFloyd, Madness

find songs -mindepth 1 | cut -d '/' -f2 | cut -d '-' -f1 | sed 's/ //g' | sort | uniq | xargs -I{} mkdir {} 
