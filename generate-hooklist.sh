#!/bin/sh

echo "/* Automatically generated by generate-hooklist.sh */"

print_hook_list () {
	cat <<EOF
static const char *hook_name_list[] = {
EOF
	perl -ne '
		chomp;
		@l[$.] = $_;
		push @h => $l[$. - 1] if /^~~~+$/s;
		END {
			print qq[\t"$_",\n] for sort @h;
		}
	' <Documentation/githooks.txt
	cat <<EOF
	NULL,
};
EOF
}

echo
print_hook_list
