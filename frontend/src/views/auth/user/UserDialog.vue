<script lang="ts" setup>
import { ref, reactive, defineProps } from 'vue'
import { newUser, updateUser } from '@/api/user'
import { ElMessage } from 'element-plus'
import type { FormRules } from 'element-plus'

const isDialogVisible = ref(false)

const props = defineProps({
  isAdd: {
    type: Boolean,
    required: true,
  }
})

const emits = defineEmits(['getList'])

const getList = () => {
  emits('getList')
}

const setIsDialogVisible = (value: boolean) => {
  isDialogVisible.value = value
}

/** 用户表单 */
const user = reactive<User>({
  user_id: 0,
  username: '',
  nickname: '',
  phone: '',
  status: 0,
  create_time: '',
  dept: {
    dept_name: '',
  },
  selected: false,
})

/** 重置用户表单 */
const resetUser = () => {
  user.user_id = 0
  user.username = ''
  user.nickname = ''
  user.phone = ''
  user.status = 0
  user.create_time = ''
  user.dept = {dept_name: ''}
  user.selected = false
}

const setUser = (newValue: User) => {
  user.user_id = newValue.user_id
  user.username = newValue.username
  user.nickname = newValue.nickname
  user.phone = newValue.phone
  user.status = newValue.status
  user.create_time = newValue.create_time
  user.dept = newValue.dept
  user.selected = false
}

/** 关闭弹窗 */
const handleClose = () => {
  setIsDialogVisible(false)
  resetUser()
}

/** 新增/修改用户提交按钮 */
const handleSave = async () => {
  if (props.isAdd) {
    const { username, nickname, phone } = user
    const new_user = {
      username,
      nickname,
      phone
    }
    const { id } = await newUser(new_user)
    user.user_id = id
    ElMessage.success('新增用户成功')
  } else {
    const { user_id, nickname, phone } = user
    const update_user: UserUpdate = {
      user_id,
      nickname,
      phone
    }
    await updateUser(update_user)
    ElMessage.success('修改用户成功')
  }
  getList()
  isDialogVisible.value = false
  resetUser()
}

/** 用户表单校验规则 */
const rules = reactive<FormRules<User>>({
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    { min: 8, max: 32, message: '长度在 8 到 32 个字符', trigger: 'blur' },
    { pattern: /^[a-zA-Z0-9]+$/, message: '用户名只能包含字母和数字', trigger: 'blur' }
  ],
  phone: [
    { pattern: /^1[3-9]\d{9}$/, message: '手机号码格式不正确', trigger: 'blur' }
  ],
});

defineExpose({
  setUser,
  setIsDialogVisible
})
</script>

<template>
  <el-dialog :title="isAdd ? '新增用户' : '修改用户'" v-model="isDialogVisible" width="400px" :before-close="handleClose">
    <el-form ref="userForm" :rules="rules" :model="user" label-width="80px">
      <el-form-item label="用户名" prop="username">
        <el-input :disabled="!isAdd" v-model="user.username" placeholder="请输入用户名" clearable style="width: 300px" />
        <el-tag type="warning">默认密码：123456</el-tag>
      </el-form-item>
      <el-form-item label="昵称" prop="nickname">
        <el-input v-model="user.nickname" placeholder="请输入昵称" clearable style="width: 300px" />
      </el-form-item>
      <el-form-item label="手机号码" prop="phone">
        <el-input v-model="user.phone" placeholder="请输入手机号码" clearable style="width: 300px" />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="handleClose">取 消</el-button>
      <el-button type="primary" @click="handleSave" v-text="isAdd ? '新增' : '修改'"></el-button>
    </template>
  </el-dialog>
</template>