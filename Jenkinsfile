// https://www.jenkins.io/doc/book/pipeline/syntax/#environment
// https://www.jenkins.io/doc/pipeline/steps/workflow-basic-steps/#pipeline-basic-steps
// Локальная информация: https://[JENKINS URL]/jenkins/pipeline-syntax/globals
pipeline {
  agent any
  stages {
    stage('Подготовка'){
      steps{
        script {
          env.HOST_WORKSPACE=env.WORKSPACE.replace(env.JENKINS_HOME,env.JENKINS_HOME_HOST_PATH)
        }
      }
    }
    stage('Клонирование') {
      when { expression { return !fileExists('.git') }}
      steps {
        sh 'git clone https://github.com/rokashevich/cp19.git .'
        sh 'git checkout master'
      }
    }
    stage('Обновление') {
      when { not { environment name: 'GIT_URL', value: 'https://github.com/rokashevich/cp19.git'}}
      steps {
        sh 'git pull --ff-only'
        sh 'git submodule update --init --recursive'
      }
    }
    stage('Parallel') {
      parallel {
        stage('Cppcheck') {
          steps {
            script {
              o = sh (
                  script: '''docker run --rm \
                            --mount type=bind,source=${HOST_WORKSPACE},target=/w \
                            rokashevich/cp19-amd64-builder bash /w/tools/cppcheck.sh
                            ''',
                  returnStdout: true
              ).trim()
              echo "${o}"
              def num_warnings = o.tokenize().last()
              push_metrics("cppcheck", num_warnings)
            }
          }
        }
        stage('Pylint') {
          steps {
            script {
              o = sh (
                  script: '''docker run --rm \
                            --mount type=bind,source=${HOST_WORKSPACE},target=/w \
                            rokashevich/cp19-amd64-builder bash /w/tools/pylint.sh
                            ''',
                  returnStdout: true
              ).trim()
              echo "${o}"
              def num_warnings = o.tokenize().last()
              push_metrics("pylint", num_warnings)
            }
          }
        }
        stage('Shellcheck') {
          steps {
            script {
              o = sh (
                  script: '''docker run --rm \
                            --mount type=bind,source=${HOST_WORKSPACE},target=/w \
                            rokashevich/cp19-amd64-builder bash /w/tools/shellcheck.sh
                            ''',
                  returnStdout: true
              ).trim()
              echo "${o}"
              def num_warnings = o.tokenize().last()
              push_metrics("shellcheck", num_warnings)
            }
          }
        }
      }
    }
    stage('Сборка Desktop') {
      steps {
        sh '''docker run --rm \
        --mount type=bind,source=${HOST_WORKSPACE},target=/w \
        rokashevich/cp19-amd64-builder bash /w/tools/build.sh -t desktop
        '''
      }
    }
    stage('Сборка Android') {
      steps {
        sh '''docker run --rm --user=root \
        --mount type=bind,source=${HOST_WORKSPACE},target=/w \
        rokashevich/cp19-android-builder bash /w/tools/build.sh -t android
        '''
      }
    }
  }
  post {
    always {
      archiveArtifacts artifacts: '_desktop/cp19', fingerprint: false
      archiveArtifacts artifacts: 'SDL/**/app-release-unsigned.apk', fingerprint: true
      push_metrics("duration_job_total", currentBuild.duration)
    }
  }
}
def push_metrics(name, value) {
  sh "echo '${name} ${value}'|curl --data-binary @- http://pushgateway-svc.my-prometheus:9091/pushgateway/metrics/job/cp19"
}