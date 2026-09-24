#!/bin/sh

set -u

minishell=${1:-./minishell}
failures=0

run_case()
{
	name=$1
	expected=$2
	actual=$("$minishell" 2>/dev/null | tail -n 2 | head -n 1)
	if [ "$actual" = "$expected" ]; then
		printf 'PASS: %s\n' "$name"
	else
		printf 'FAIL: %s\n' "$name"
		printf '  expected: <%s>\n' "$expected"
		printf '  actual:   <%s>\n' "$actual"
		failures=$((failures + 1))
	fi
}

run_case "unquoted delimiter expands exported variables" "42_is_awesome" <<'CASE'
export TEST_VAR=42_is_awesome
cat << EOF
$TEST_VAR
EOF
exit
CASE

run_case "single-quoted delimiter suppresses expansion" '$TEST_VAR' <<'CASE'
export TEST_VAR=42_is_awesome
cat << 'EOF'
$TEST_VAR
EOF
exit
CASE

run_case "mixed-quoted delimiter suppresses expansion" '$TEST_VAR' <<'CASE'
export TEST_VAR=42_is_awesome
cat << E"OF"
$TEST_VAR
EOF
exit
CASE

run_case "opposite quotes in delimiter stay literal" "nested" <<'CASE'
cat << "'EOF'"
nested
'EOF'
exit
CASE

run_case "body quotes stay literal while variables expand" \
	'quotes: '\''42_is_awesome'\'' "42_is_awesome" *' <<'CASE'
export TEST_VAR=42_is_awesome
cat << EOF
quotes: '$TEST_VAR' "$TEST_VAR" *
EOF
exit
CASE

run_case "last of multiple heredocs provides input" "second" <<'CASE'
cat << FIRST << SECOND
first
FIRST
second
SECOND
exit
CASE

if [ "$failures" -ne 0 ]; then
	printf '%s heredoc regression(s) failed\n' "$failures" >&2
	exit 1
fi

printf 'All heredoc issue regressions passed\n'
