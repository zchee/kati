// Copyright 2015 Google Inc. All rights reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package kati

import (
	"bytes"
	"fmt"
	"os"
	"sync"
)

var logMu sync.Mutex

func LogAlways(f string, a ...interface{}) {
	var buf bytes.Buffer
	buf.WriteString("*kati*: ")
	buf.WriteString(f)
	buf.WriteByte('\n')
	logMu.Lock()
	fmt.Printf(buf.String(), a...)
	logMu.Unlock()
}

func LogStats(f string, a ...interface{}) {
	if !LogFlag && !StatsFlag {
		return
	}
	LogAlways(f, a...)
}

func Logf(f string, a ...interface{}) {
	if !LogFlag {
		return
	}
	LogAlways(f, a...)
}

func Warn(filename string, lineno int, f string, a ...interface{}) {
	f = fmt.Sprintf("%s:%d: warning: %s\n", filename, lineno, f)
	fmt.Printf(f, a...)
}

func WarnNoPrefix(filename string, lineno int, f string, a ...interface{}) {
	f = fmt.Sprintf("%s:%d: %s\n", filename, lineno, f)
	fmt.Printf(f, a...)
}

var atErrors []func()

func AtError(f func()) {
	atErrors = append(atErrors, f)
}

func Error(filename string, lineno int, f string, a ...interface{}) {
	f = fmt.Sprintf("%s:%d: %s", filename, lineno, f)
	ErrorNoLocation(f, a...)
}

func ErrorNoLocation(f string, a ...interface{}) {
	f = fmt.Sprintf("%s\n", f)
	fmt.Printf(f, a...)
	for i := len(atErrors) - 1; i >= 0; i-- {
		atErrors[i]()
	}
	os.Exit(2)
}
