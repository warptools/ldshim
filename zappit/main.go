package main

import (
	"flag"
	"log"
	"os"
	"path/filepath"

	"github.com/u-root/u-root/pkg/ldd"
	"github.com/u-root/uio/cp"
	"golang.org/x/exp/slices"
)

var (
	outDir = flag.String("out", ".", "Directory in which to create lib, dynbin, and bin output subdirectories")
	shim   = flag.String("shim", "", "Path to shim binary (required)")
)

func main() {
	flag.Parse()

	if flag.NArg() != 1 {
		log.Fatal("Must specify one command to zapp -- zappit --shim=PATH [CMD]")
	}
	if len(*shim) == 0 {
		log.Fatal("Must specify --shim=PATH-TO-SHIM")
	}

	cmd := flag.Arg(0)

	deps, err := ldd.List(cmd)
	if err != nil {
		log.Fatalf("Could not determine dependencies of %s: %v", cmd, err)
	}
	if !slices.ContainsFunc(deps, func(name string) bool {
		return filepath.Base(name) == "ld-linux-x86-64.so.2"
	}) {
		// TODO: This limitation is hard-coded in the shim.
		log.Fatalf("Shared library application must use ld-linux-x86-64.so.2 as its ld.so")
	}

	if err := os.MkdirAll(*outDir, 0777); err != nil && !os.IsExist(err) {
		log.Fatalf("Output directory could not be created: %v", err)
	}
	if err := os.Mkdir(filepath.Join(*outDir, "lib"), 0777); err != nil && !os.IsExist(err) {
		log.Fatalf("Output directory could not be created: %v", err)
	}
	if err := os.Mkdir(filepath.Join(*outDir, "bin"), 0777); err != nil && !os.IsExist(err) {
		log.Fatalf("Output directory could not be created: %v", err)
	}
	if err := os.Mkdir(filepath.Join(*outDir, "dynbin"), 0777); err != nil && !os.IsExist(err) {
		log.Fatalf("Output directory could not be created: %v", err)
	}
	// Put the shim at $out/bin/$cmd
	if err := cp.Copy(*shim, filepath.Join(*outDir, "bin", filepath.Base(cmd))); err != nil {
		log.Fatal(err)
	}
	// Put the cmd at $out/dynbin/$cmd
	if err := cp.Copy(cmd, filepath.Join(*outDir, "dynbin", filepath.Base(cmd))); err != nil {
		log.Fatal(err)
	}

	// Copy dynamic dependencies to $out/lib
	slices.Sort(deps)
	for _, dep := range deps {
		if err := cp.Copy(dep, filepath.Join(*outDir, "lib", filepath.Base(dep))); err != nil {
			log.Fatal(err)
		}
	}
}
