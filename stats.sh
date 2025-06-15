#!/usr/bin/env bash
set -euo pipefail

# ── configure these ────────────────────────────────────────────────
# list of dirs (relative to repo root) to skip entirely
IGNORE_DIRS=(
  .git
  dist
  build
  build-gcc
  gcc-cross-compiler
  docs
)
# extensions → human-readable labels
declare -A types=(
  ["cfg"]="Config"
  ["c"]="C"
  ["h"]="C Header"
  ["asm"]="Assembly"
  ["ld"]="Linker"
)
# ── end config ────────────────────────────────────────────────────

prune_expr=()
for d in "${IGNORE_DIRS[@]}"; do
  prune_expr+=( -path "./$d" -o )
done
unset 'prune_expr[${#prune_expr[@]}-1]'

echo "Project line count per language (skipping: ${IGNORE_DIRS[*]})"
echo "----------------------------------------------------------------"

grand_total=0

for ext in "${!types[@]}"; do
  count=0
  while IFS= read -r -d '' file; do
    lines=$(wc -l < "$file")
    count=$((count + lines))
  done < <(
    find . \
      \( "${prune_expr[@]}" \) -prune -o \
      -type f -name "*.${ext}" -print0
  )

  printf "%-12s: %6d lines\n" "${types[$ext]}" "$count"
  grand_total=$((grand_total + count))
done

echo "----------------------------------------------------------------"
printf "%-12s: %6d lines\n" "TOTAL" "$grand_total"