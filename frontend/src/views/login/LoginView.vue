<script setup lang="ts">
import { reactive, nextTick } from 'vue';
import { type FormRules } from 'element-plus';
import { type LoginForm, login } from '@/api/auth';
import { useCommonStore } from '@/stores/common';
import router from '@/router';

const form = reactive<LoginForm>({
  username: '',
  password: ''
});

const rules = reactive<FormRules<LoginForm>>({
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    { min: 8, max: 32, message: '长度在 8 到 32 个字符', trigger: 'blur' },
    { pattern: /^[a-zA-Z0-9]+$/, message: '用户名只能包含字母和数字', trigger: 'blur' }
  ],
  password: [
    { required: true, message: '请输入密码', trigger: 'blur' },
    { min: 6, max: 16, message: '长度在 6 到 16 个字符', trigger: 'blur' },
    { pattern: /^[a-zA-Z0-9]+$/, message: '密码只允许包含字母和数字', trigger: 'blur' }
  ]
});

// useCommonStore
const commonStore = useCommonStore();
const { setToken } = commonStore

const handleLogin = async () => {
  const res = await login(form.username, form.password)
  setToken(res.token)
  nextTick(() => {
    router.go(-1)
  })
}
</script>

<template>
  <div class="login-view">
    <el-card>
      <template #header><h1>登录页面</h1></template>
      <el-form :model="form" :rules="rules" label-width="70px" label-position="left">
        <el-form-item label="用户名" prop="username">
          <el-input v-model="form.username" placeholder="请输入用户名"></el-input>
        </el-form-item>
        <el-form-item label="密码" prop="password">
          <el-input type="password" v-model="form.password" placeholder="请输入密码" show-password></el-input>
        </el-form-item>
      </el-form>
      <template #footer class="footer">
        <el-button type="primary" size="large" @click="handleLogin">登录</el-button>
      </template>
    </el-card>
  </div>
</template>

<style scoped lang="scss">
.login-view {
  height: 100%;
  width: 100%;
  background-color: #ebf0ff;
  display: flex;
  justify-content: center;
  align-items: center;
  .el-card {
    width: 340px;
    height: 254px;
    :deep(.el-card__body) {
      padding-bottom: 0;
    }
    :deep(.el-card__footer) {
      display: flex;
      justify-content: center;
    }
  }
}
</style>
