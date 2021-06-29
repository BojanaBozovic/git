#include "test-tool.h"
#include "cache.h"
#include "oidtree.h"

static enum cb_next print_oid(const struct object_id *oid, void *data)
{
	puts(oid_to_hex(oid));
	return CB_CONTINUE;
}

int cmd__oidtree(int argc, const char **argv)
{
	struct oidtree ot = OIDTREE_INIT;
	struct strbuf line = STRBUF_INIT;
	int nongit_ok;

	setup_git_directory_gently(&nongit_ok);

	while (strbuf_getline(&line, stdin) != EOF) {
		const char *arg;
		struct object_id oid;

		if (skip_prefix(line.buf, "insert ", &arg)) {
			if (get_oid_hex(arg, &oid))
				die("not a hexadecimal oid: %s", arg);
			oidtree_insert(&ot, &oid);
		} else if (skip_prefix(line.buf, "contains ", &arg)) {
			if (get_oid_hex(arg, &oid))
				die("not a hexadecimal oid: %s", arg);
			printf("%d\n", oidtree_contains(&ot, &oid));
		} else if (skip_prefix(line.buf, "each ", &arg)) {
			char buf[GIT_SHA1_HEXSZ  + 1] = { '0' };
			memset(&oid, 0, sizeof(oid));
			memcpy(buf, arg, strlen(arg));
			buf[GIT_SHA1_HEXSZ] = 0;
			get_oid_hex_any(buf, &oid);
			oid.algo = GIT_HASH_SHA1;
			oidtree_each(&ot, &oid, strlen(arg), print_oid, NULL);
		} else if (!strcmp(line.buf, "destroy"))
			oidtree_destroy(&ot);
		else
			die("unknown command: %s", line.buf);
	}
	return 0;
}
